#ifndef ANNOUNCE_H
#define ANNOUNCE_H

#include <vector>
#include <cstdint>

struct agent {
    //Both of these vectors are in DNF
    std::vector<std::vector<int32_t>> beliefs;
    std::vector<std::vector<int32_t>> goal;
};

void find_announcement(const std::vector<agent>& agents) noexcept;

bool goals_consistent(const std::vector<std::vector<std::vector<int32_t>>>& goals) noexcept;


#endif
