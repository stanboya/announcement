#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <utility>
#include <bitset>
#include <unordered_set>

#include "utils.h"

std::vector<std::vector<bool>> convert_to_bool(const std::vector<std::vector<int32_t>>& state) noexcept {
    std::vector<std::vector<bool>> output;

    for (const auto& clause : state) {
        const auto max_term = std::abs(*std::max_element(clause.cbegin(), clause.cend(), [](const auto& lhs, const auto& rhs){return std::abs(lhs) < std::abs(rhs);}));
        std::vector<bool> converted_term{static_cast<unsigned long>(max_term), false, std::allocator<bool>()};
        for (const auto term : clause) {
            converted_term[std::abs(term) - 1] = (term > 0);
        }
        output.emplace_back(std::move(converted_term));
    }

    return output;
}

std::vector<std::vector<int32_t>> convert_to_num(const std::vector<std::vector<bool>>& state) noexcept {
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

void print_formula_dnf(const std::vector<std::vector<int32_t>>& formula) noexcept {
    for (const auto& clause : formula) {
        std::cout << '(';
        for (const auto term : clause) {
            if (term < 0) {
                std::cout << "not ";
            }
            std::cout << std::abs(term);
            if (term != clause.back()) {
                std::cout << " and ";
            }
        }
        std::cout << ')';
        if (clause != formula.back()) {
            std::cout << " or ";
        }
    }
    std::cout << std::endl;
}

//This applies the distributive property to convert between DNF and CNF DIMACS formats
std::vector<std::vector<int32_t>> convert_normal_forms(const std::vector<std::vector<int32_t>>& normal_clauses) noexcept {
    std::vector<std::vector<int32_t>> result;

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
        const auto prev_result = convert_normal_forms({std::next(normal_clauses.begin()), normal_clauses.end()});
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
            std::sort(clause.begin(), clause.end(), [](const auto lhs, const auto rhs){return std::abs(lhs) < std::abs(rhs);});
        }
        return result;
    }
}

//Converts the input bits to DNF format - It's a 1-to-1 mapping
std::vector<std::vector<int32_t>> convert_raw(const std::vector<std::vector<bool>>& input_bits) noexcept {
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
std::vector<std::vector<bool>> convert_dnf_to_raw(const std::vector<std::vector<int32_t>>& clause_list) noexcept {
    std::unordered_set<std::vector<bool>> output_set;
    output_set.reserve(clause_list.size());

    int32_t variable_count = INT32_MIN;
#pragma omp parallel shared(output_set, variable_count)
    {
#pragma omp for schedule(static) reduction(max: variable_count) nowait
        for (auto it = clause_list.cbegin(); it < clause_list.cend(); ++it) {
            for (auto it2 = it->cbegin(); it2 < it->cend(); ++it2) {
                variable_count = std::max(variable_count, std::abs(*it2));
            }
        }

#pragma omp for schedule(static) nowait
        for (auto it = clause_list.cbegin(); it < clause_list.cend(); ++it) {
            //Fill the converted state to have variable_count falses
            std::vector<bool> converted_state{static_cast<unsigned long>(variable_count), false, std::allocator<bool>()};

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

    std::vector<std::vector<bool>> output{std::make_move_iterator(output_set.begin()), std::make_move_iterator(output_set.end())};

    return output;
}

