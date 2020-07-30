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
#include <iterator>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <bitset>

#include "announce.h"
#include "interactive.h"
#include "tools.h"
#include "utils.h"

std::unordered_set<int32_t> get_terms_from_DNF(const std::vector<std::vector<int32_t>> clauses) {
    std::unordered_set<int32_t> terms{};
    std::unordered_set<int32_t> removed_terms{};
    for(const auto& clause : clauses) {
        for(const auto term : clause) {
            if(removed_terms.find(std::abs(term)) == removed_terms.end()) {
                if(terms.find(-term) != terms.end()) {
                    terms.erase(-term);
                    removed_terms.insert(std::abs(term));
                }
                else {
                    terms.insert(term);
                }
            }
        }
    }

    return terms;
}

std::string find_announcement_KB(const std::vector<agent>& agents) noexcept {

    std::vector<std::vector<int32_t>> phi_belief_state_dnf{};
    
    std::vector<unsigned long> minimum_hamming_distances{};
    

    std::vector<std::vector<std::vector<bool>>> K_beliefs{}, G_beliefs{};
    for(const auto& agent : agents) {
        std::vector<std::vector<int32_t>> agentBelief_converted_form{};
        for(size_t i = 0; i < agent.beliefs.front().size(); i++) {
            std::vector<int32_t> cnf_clause{};
            for (const auto clause : agent.beliefs) {
                
                cnf_clause.emplace_back(clause[i]);
                
            }
            agentBelief_converted_form.emplace_back(std::move(cnf_clause));
        }
        K_beliefs.push_back(convert_dnf_to_raw(allsat(agentBelief_converted_form)));
        print_formula_dnf(agent.goal);

        std::vector<std::vector<int32_t>> agentGoal_converted_form{};
        for(size_t i = 0; i < agent.goal.front().size(); i++) {
            std::vector<int32_t> cnf_clause{};
            for (const auto clause : agent.goal) {
                
                cnf_clause.emplace_back(clause[i]);
                
            }
            agentGoal_converted_form.emplace_back(std::move(cnf_clause));
        }
        G_beliefs.push_back(convert_dnf_to_raw(allsat(agentGoal_converted_form)));
    }

    

    for(size_t i{ 0 }; i < K_beliefs.size(); i++) {
        
        unsigned long minimumHammingDistance{ std::numeric_limits<unsigned long>::max() };
        for(const auto& G_belief : G_beliefs[i]) {
            
            auto hamming_distance = state_difference(G_belief, K_beliefs[i]);
            
            if(minimumHammingDistance > hamming_distance) {
                minimumHammingDistance = hamming_distance;
            }
        }

        minimum_hamming_distances.push_back(minimumHammingDistance);
    }

    
    
    std::vector<std::vector<bool>> phi_belief_state{};
    for(size_t i{ 0 }; i < G_beliefs.size(); i++) {

        for(const auto& state : G_beliefs[i]) {
            if(std::find(phi_belief_state.begin(), phi_belief_state.end(), state) == phi_belief_state.end()) {
                // bool state_possible{ true };
                // for(size_t j = 0; j < K_beliefs.size(); j++) {
                //     if(state_difference(state, K_beliefs[j]) == minimum_hamming_distances[j] && std::find(G_beliefs[j].begin(), G_beliefs[j].end(), state) == G_beliefs[j].end()) {
                //         return "No possible satisfying assignment was found\n";
                //     }
                // }
                // if(state_possible) {
                    phi_belief_state.push_back(state);
                // }    
            }
        }
    }

    for(auto state : phi_belief_state) {
        for(auto var : state) {
            std::cout << var << " ";
        }

        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    if (phi_belief_state.empty() || !test_announcement(agents, convert_raw(phi_belief_state))) {
        return "No possible satisfying assignment was found\n";
    }
    else {
        phi_belief_state_dnf = convert_raw(phi_belief_state);
        
        simplify_dnf(phi_belief_state_dnf);
        return print_formula_dnf(minimize_output(phi_belief_state_dnf));
        
    }

}

std::string find_announcement(const std::vector<agent>& agents) noexcept {
    std::vector<std::vector<std::vector<int32_t>>> goals;

    for (const auto& agent : agents) {
        goals.emplace_back(agent.goal);
    }

    if (goals_consistent(goals)) {
        std::cout << "Goals are consistent\n";

        if (verbose) {
            for (const auto& agent : agents) {
                for (const auto& clause : agent.goal) {
                    for (const auto term : clause) {
                        std::cout << term << " ";
                    }
                    std::cout << "\n";
                }
                std::cout << "\n";
            }
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

    for (auto i = 1; i <= max_var; ++i) {
        std::vector<bool> v(max_var);
        std::fill(v.begin(), v.begin() + i, true);

        //Generate all length i combinations of variables
        do {
            std::vector<int32_t> combination;
            for (int j = 0; j < max_var; ++j) {
                if (v[j]) {
                    combination.emplace_back(j + 1);
                }
            }

            //Generate all variable negation variations
            for (uint64_t j = 0; j < (1ul << i); ++j) {
                std::vector<int32_t> negated_comb{combination};
                for (auto k = 0; k < i; ++k) {
                    const auto val = (1 << k) & j;
                    if (!val) {
                        negated_comb[k] *= -1;
                    }
                }

                //Generate all conjunction permutations
                for (uint64_t k = 0; k < (1ul << (i - 1)); ++k) {
                    std::vector<bool> conjunctions;
                    for (auto l = 0; l < (i - 1); ++l) {
                        const auto val = (1 << l) & k;
                        conjunctions.emplace_back(val);
                    }

                    std::vector<std::vector<int32_t>> revision_formula;
                    for (auto l = 0; l < i; ++l) {
                        if (l == 0) {
                            revision_formula.push_back({negated_comb[0]});
                        } else {
                            if (conjunctions[l - 1]) {
                                //AND
                                revision_formula.back().push_back(negated_comb[l]);
                            } else {
                                //OR
                                revision_formula.push_back({negated_comb[l]});
                            }
                        }
                    }
                    if (test_announcement(agents, revision_formula)) {
                        std::cout << "GOOD Found an announcement that works\n";
                        return print_formula_dnf(revision_formula);
                    }
                }
            }
        } while (std::prev_permutation(v.begin(), v.end()));
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

    for (auto& clause : conjunction) {
        //Sort the conjunction in variable order
        std::sort(clause.begin(), clause.end(),
                [](const auto lhs, const auto rhs) { return std::abs(lhs) < std::abs(rhs); });
    }
    std::sort(conjunction.begin(), conjunction.end());
    conjunction.erase(std::unique(conjunction.begin(), conjunction.end()), conjunction.end());
    conjunction.erase(std::remove_if(conjunction.begin(), conjunction.end(),
                              [](const auto& clause) { return clause.empty(); }),
            conjunction.end());
    conjunction.shrink_to_fit();

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
                max_variable_count = std::max(max_variable_count, std::abs(term));
            }
        }
        for (const auto& clause : ag.goal) {
            for (const auto term : clause) {
                max_variable_count = std::max(max_variable_count, std::abs(term));
            }
        }
    }
    return max_variable_count;
}

bool test_announcement(const std::vector<agent>& agents,
        const std::vector<std::vector<int32_t>>& revision_formula) noexcept {
    
    for (const auto& agent : agents) {
        auto agentBeliefs = agent.beliefs;
        
        auto revised = belief_revise(agentBeliefs, revision_formula);
        const auto abs_cmp = [](const auto a, const auto b) { return std::abs(a) < std::abs(b); };
        
        for (auto& clause : revised) {
            std::sort(clause.begin(), clause.end(), abs_cmp);
        }
        std::sort(revised.begin(), revised.end());

        
        std::cout << "\n";
        std::cout << "Agent beliefs: ";
        print_formula_dnf(agent.beliefs);
    
        std::cout << "Revision formula: ";
        print_formula_dnf(revision_formula);

        std::cout << "Revised output: "; 
        print_formula_dnf(revised);

        std::cout << "Agent goal: ";
        print_formula_dnf(agent.goal);

        if(revised.empty() && !agent.goal.empty()) {
            return false;
        }

        for (const auto& clause : revised) {
            if (std::find(agent.goal.cbegin(), agent.goal.cend(), clause) == agent.goal.cend()) {
                return false;
            }
        }
    }
    return true;
}
