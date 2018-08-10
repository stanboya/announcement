/**
 Copyright   John Agapeyev 2018

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>
#include <sstream>

#include "utils.h"

std::vector<std::vector<bool>> convert_to_bool(
        const std::vector<std::vector<int32_t>>& state) noexcept {
    std::vector<std::vector<bool>> output;

    for (const auto& clause : state) {
        const auto max_term = std::abs(*std::max_element(clause.cbegin(), clause.cend(),
                [](const auto& lhs, const auto& rhs) { return std::abs(lhs) < std::abs(rhs); }));
        std::vector<bool> converted_term{
                static_cast<unsigned long>(max_term), false, std::allocator<bool>()};
        for (const auto term : clause) {
            converted_term[std::abs(term) - 1] = (term > 0);
        }
        output.emplace_back(std::move(converted_term));
    }

    return output;
}

std::vector<std::vector<int32_t>> convert_to_num(
        const std::vector<std::vector<bool>>& state) noexcept {
    std::vector<std::vector<int32_t>> output;

    for (const auto& clause : state) {
        std::vector<int32_t> converted_term;
        for (unsigned long i = 0; i < clause.size(); ++i) {
            int32_t term = i + 1;
            if (!clause[i]) {
                term *= -1;
            }
            converted_term.emplace_back(term);
        }
        output.emplace_back(std::move(converted_term));
    }

    return output;
}

std::string print_formula_dnf(const std::vector<std::vector<int32_t>>& formula) noexcept {
    std::stringstream ss;
    for (const auto& clause : formula) {
        for (const auto term : clause) {
            ss << term << " ";
        }
        ss << "\n";
    }

    for (const auto& clause : formula) {
        ss << '(';
        for (const auto term : clause) {
            if (term < 0) {
                ss << "not ";
            }
            ss << std::abs(term);
            if (term != clause.back()) {
                ss << " and ";
            }
        }
        ss << ')';
        if (clause != formula.back()) {
            ss << " or ";
        }
    }
    ss << std::endl;
    std::cout << ss.str();
    return ss.str();
}

//This applies the distributive property to convert between DNF and CNF DIMACS formats
std::vector<std::vector<int32_t>> convert_normal_forms(
        const std::vector<std::vector<int32_t>>& normal_clauses) noexcept {
    std::vector<std::vector<int32_t>> result;

    if (normal_clauses.empty()) {
        return normal_clauses;
    }

    std::cout << "Conversion called with size " << normal_clauses.size() << "\n";

    assert(normal_clauses.size() >= 2);

    if (normal_clauses.size() == 2) {
        //Handle final case
        for (const auto& first : normal_clauses.front()) {
            for (const auto second : normal_clauses[1]) {
                result.push_back({first, second});
            }
        }

        return result;
    } else {
        //Get the result excluding the first clause
        const auto prev_result
                = convert_normal_forms({std::next(normal_clauses.begin()), normal_clauses.end()});
        for (const auto term : normal_clauses.front()) {
            for (const auto& clause : prev_result) {
                //Add the first level term to the existing conversion
                std::vector<int32_t> extended_result{clause};
                extended_result.emplace_back(term);
                result.emplace_back(std::move(extended_result));
            }
        }
        for (auto& clause : result) {
            //Sort the result in variable order
            std::sort(clause.begin(), clause.end(),
                    [](const auto lhs, const auto rhs) { return std::abs(lhs) < std::abs(rhs); });
        }
        std::sort(result.begin(), result.end());
        result.erase(std::unique(result.begin(), result.end()), result.end());
        result.erase(std::remove_if(result.begin(), result.end(),
                             [](const auto& clause) { return clause.empty(); }),
                result.end());
        result.shrink_to_fit();

        std::cout << "Step finished\n";
        std::cout << "New size " << result.size() << "\n";
        return result;
    }
}

//Converts the input bits to DNF format - It's a 1-to-1 mapping
std::vector<std::vector<int32_t>> convert_raw(
        const std::vector<std::vector<bool>>& input_bits) noexcept {
    std::vector<std::vector<int32_t>> output;
    output.reserve(input_bits.size());

    for (const auto& clause : input_bits) {
        std::vector<int32_t> converted_form;

        for (unsigned long i = 0; i < clause.size(); ++i) {
            converted_form.push_back((clause[i]) ? i + 1 : -(i + 1));
        }
        output.emplace_back(std::move(converted_form));
    }

    return output;
}

//This needs to pad for all unset variables
//So if each clause doesn't specify every variable's state, then theoretically it can be in any state
//Eg. A and B only specifies the first 2 bits, but if there are 8 variables, then those other 6 could be any state, hence the need for padding
//So padding will be necessary, unfortunately
//This will greatly slow things down if the examples are simple
std::vector<std::vector<bool>> convert_dnf_to_raw(
        const std::vector<std::vector<int32_t>>& clause_list) noexcept {
    std::unordered_set<std::vector<bool>> output_set;
    output_set.reserve(clause_list.size());

    int32_t variable_count = INT32_MIN;
#pragma omp parallel shared(output_set, variable_count)
    {
#pragma omp for schedule(static) reduction(max : variable_count) nowait
        for (auto it = clause_list.cbegin(); it < clause_list.cend(); ++it) {
            for (auto it2 = it->cbegin(); it2 < it->cend(); ++it2) {
                variable_count = std::max(variable_count, std::abs(*it2));
            }
        }

#pragma omp for schedule(static) nowait
        for (auto it = clause_list.cbegin(); it < clause_list.cend(); ++it) {
            //Fill the converted state to have variable_count falses
            std::vector<bool> converted_state{
                    static_cast<unsigned long>(variable_count), false, std::allocator<bool>()};

            std::unordered_set<int32_t> variable_set;
            for (const auto term : *it) {
                variable_set.emplace(std::abs(term) - 1);

                //Fill in the bits set by the clause
                converted_state[std::abs(term) - 1] = (term > 0);
            }
            int32_t clause_max = variable_set.size();

            //Brute force pad the unused bits
            for (uint64_t mask = 0; mask < (1ull << (variable_count - clause_max)); ++mask) {
                std::bitset<64> bs{mask};

                unsigned long pos = 0;
                for (int32_t i = 0; i < variable_count; ++i) {
                    //Variable at position i is not set by the clause
                    if (!variable_set.count(i)) {
                        converted_state[i] = bs[pos++];
                    }
                }
#pragma omp critical
                output_set.emplace(converted_state);
            }
        }
    }

    std::vector<std::vector<bool>> output{
            std::make_move_iterator(output_set.begin()), std::make_move_iterator(output_set.end())};

    return output;
}

void simplify_dnf(std::vector<std::vector<int32_t>>& formula) noexcept {
    const auto abs_cmp = [](const auto a, const auto b) { return std::abs(a) < std::abs(b); };

    //Remove duplicates and empties, while sorting the 2d vector
    for (auto& clause : formula) {
        std::sort(clause.begin(), clause.end(), abs_cmp);
        clause.erase(std::unique(clause.begin(), clause.end()), clause.end());

        std::vector<int32_t> remove_vals;
        for (const auto term : clause) {
            if (std::find(clause.cbegin(), clause.cend(), term * -1) != clause.cend()) {
                remove_vals.emplace_back(term);
            }
        }

        if (remove_vals.empty()) {
            continue;
        }

        clause.erase(
                std::remove_if(clause.begin(), clause.begin(),
                        [&](const auto term) {
                            return std::find(remove_vals.begin(), remove_vals.end(), std::abs(term))
                                    != remove_vals.end();
                        }),
                clause.end());
    }
    std::sort(formula.begin(), formula.end());
    formula.erase(std::unique(formula.begin(), formula.end()), formula.end());
    formula.erase(std::remove_if(formula.begin(), formula.end(),
                          [](const auto& clause) { return clause.empty(); }),
            formula.end());
}
