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
#ifndef ANNOUNCE_H
#define ANNOUNCE_H

#include <cstdint>
#include <string>
#include <vector>

struct agent {
    //Both of these vectors are in DNF
    std::vector<std::vector<int32_t>> beliefs;
    std::vector<std::vector<int32_t>> goal;
};

std::string find_announcement(const std::vector<agent>& agents) noexcept;

//Goals must be vector of DNF formulas
bool goals_consistent(const std::vector<std::vector<std::vector<int32_t>>>& goals) noexcept;

int32_t get_variable_count(const std::vector<agent>& agents) noexcept;

bool test_announcement(const std::vector<agent>& agents, const std::vector<std::vector<int32_t>>& revision_formula) noexcept;

#endif
