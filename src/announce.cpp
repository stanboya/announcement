#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include "tools.h"
#include "announce.h"

void find_announcement(const std::vector<agent>& agents) noexcept {

}


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

    conjunction = convert_normal_forms(conjunction);

    const auto abs_cmp = [](const auto a, const auto b){return std::abs(a) < std::abs(b);};

    //Remove duplicates and empties, while sorting the 2d vector
    for (auto& clause : conjunction) {
        std::sort(clause.begin(), clause.end(), abs_cmp);
        clause.erase(std::unique(clause.begin(), clause.end()), clause.end());
    }
    std::sort(conjunction.begin(), conjunction.end());
    conjunction.erase(std::unique(conjunction.begin(), conjunction.end()), conjunction.end());
    conjunction.erase(std::remove_if(conjunction.begin(), conjunction.end(), [](const auto& clause){return clause.empty();}), conjunction.end());

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
