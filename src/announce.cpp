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
#include <atomic>
#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>

#include "announce.h"
#include "interactive.h"
#include "tools.h"
#include "utils.h"

std::string find_announcement(const std::vector<agent>& agents) noexcept {
    std::vector<std::vector<std::vector<int32_t>>> goals;

    for (const auto& agent : agents) {
        goals.emplace_back(agent.goal);
    }

    if (goals_consistent(goals)) {
        std::cout << "Goals are consistent\n";

        for (const auto& agent : agents) {
            for (const auto& clause : agent.goal) {
                for (const auto term : clause) {
                    std::cout << term << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }

        std::vector<std::vector<int32_t>> conjunction;
        std::vector<std::vector<int32_t>> base_goal{goals.front()};

        for (const auto& goal : goals) {
            if (goal == goals.front()) {
                continue;
            }
            for (const auto& base_clause : base_goal) {
                for (const auto& clause : goal) {
                    std::vector<int32_t> appended{base_clause};
                    appended.insert(appended.end(), clause.begin(), clause.end());
                    conjunction.emplace_back(std::move(appended));
                }
            }
        }

        //Conjunction will be empty when the only goal is a single term function
        if (conjunction.empty()) {
            conjunction = std::move(base_goal);
        }
        simplify_dnf(conjunction);

        return print_formula_dnf(conjunction);
    }

    std::cout << "Goals are inconsistent, trying to find a solution\n";

    const auto max_var = get_variable_count(agents);
    assert(max_var <= 64);

    std::string ret_str;

    std::atomic_bool is_done = false;

#pragma omp parallel for schedule(static)
    for (uint64_t i = 0; i < (1ul << (max_var)); ++i) {
        if (is_done) {
            continue;
        }
        std::vector<int32_t> test_case;
        for (auto j = 0; j < max_var; ++j) {
            const auto val = (1 << j) & i;
            if (val) {
                test_case.emplace_back((j + 1));
            } else {
                test_case.emplace_back(-(j + 1));
            }
        }
        //Now test_case is a n bit brute force generating DNF formulas
        //Need to do belief revision on this and compare with goals
        std::vector<std::vector<int32_t>> revision_formula;
        revision_formula.emplace_back(std::move(test_case));

        bool bad_solution = false;

        for (const auto& agent : agents) {
            auto revised = belief_revise(agent.beliefs, revision_formula);
            const auto abs_cmp
                    = [](const auto a, const auto b) { return std::abs(a) < std::abs(b); };
            for (auto& clause : revised) {
                std::sort(clause.begin(), clause.end(), abs_cmp);
            }
            std::sort(revised.begin(), revised.end());

            if (verbose) {
                std::cout << "\n";
                std::cout << "Agent beliefs: \n";
                print_formula_dnf(agent.beliefs);

                std::cout << "Revision formula: \n";
                print_formula_dnf(revision_formula);

                std::cout << "Revised output: \n";
                print_formula_dnf(revised);

                std::cout << "Agent goal: \n";
                print_formula_dnf(agent.goal);
            }

            for (const auto& clause : agent.goal) {
                if (std::find(revised.cbegin(), revised.cend(), clause) == revised.cend()) {
                    bad_solution = true;
                    break;
                }
            }
            if (bad_solution) {
                break;
            }
        }
        if (bad_solution) {
            continue;
        }
        std::cout << "Found an announcement that works\n";
        const auto revised_beliefs = convert_dnf_to_raw(revision_formula);
        for (const auto& first : revised_beliefs) {
            for (const auto& second : revised_beliefs) {
                if (first == second) {
                    continue;
                }
                unsigned long count = 0;
                for (unsigned long k = 0; k < first.size(); ++k) {
                    count += first[k] ^ second[k];
                }
                if (count == 1) {
                    std::cout << "Minimization is possible\n";
                    ret_str = get_minimal_formula(revised_beliefs);
                    is_done = true;
                }
            }
        }
        //Prevent overwriting return string if minimization was done
        if (is_done) {
            continue;
        }
        ret_str = print_formula_dnf(revision_formula);
        is_done = true;
    }
    if (!ret_str.empty()) {
        return ret_str;
    }

    std::cout << "No possible satisfying assignment was found\n";
    return "No possible satisfying assignment was found\n";
}

//Goals must be vector of DNF formulas
bool goals_consistent(const std::vector<std::vector<std::vector<int32_t>>>& goals) noexcept {
    std::vector<std::vector<int32_t>> conjunction;

    std::vector<std::vector<int32_t>> base_goal{goals.front()};

    for (const auto& goal : goals) {
        if (goal == goals.front()) {
            continue;
        }
        for (const auto& base_clause : base_goal) {
            for (const auto& clause : goal) {
                std::vector<int32_t> appended{base_clause};
                appended.insert(appended.end(), clause.begin(), clause.end());
                conjunction.emplace_back(std::move(appended));
            }
        }
    }

    //Conjunction will be empty when the only goal is a single term function
    if (conjunction.empty()) {
        conjunction = std::move(base_goal);
    }

    simplify_dnf(conjunction);

    if (conjunction.size() == 1) {
        //Convert single clause DNF to CNF
        std::vector<std::vector<int32_t>> converted_form;
        for (const auto clause : conjunction.front()) {
            std::vector<int32_t> cnf_clause;
            cnf_clause.emplace_back(clause);
            converted_form.emplace_back(std::move(cnf_clause));
        }
        conjunction = converted_form;
    } else {
        conjunction = convert_normal_forms(conjunction);
    }

    //simplify_dnf(conjunction);

    if (conjunction.empty()) {
        return false;
    }

    std::cout << "Goal consistency input to SAT solver:\n";
    for (const auto& clause : conjunction) {
        for (const auto term : clause) {
            std::cout << term << " ";
        }
        std::cout << "\n";
    }

    return sat(conjunction);
}

int32_t get_variable_count(const std::vector<agent>& agents) noexcept {
    int32_t max_variable_count = INT32_MIN;
    for (const auto& ag : agents) {
        for (const auto& clause : ag.beliefs) {
            for (const auto term : clause) {
                max_variable_count = std::max(max_variable_count, term);
            }
        }
    }
    return max_variable_count;
}
