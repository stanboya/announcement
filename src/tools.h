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
#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <cstdint>
#include <bitset>
#include <unordered_map>

bool sat(const std::vector<std::vector<int32_t>>& formula) noexcept;
std::vector<std::vector<int32_t>> allsat(const std::vector<std::vector<int32_t>>& formula) noexcept;
std::vector<std::vector<int32_t>> belief_revise(const std::vector<std::vector<int32_t>>& beliefs, const std::vector<std::vector<int32_t>>& revision_formula) noexcept;
std::vector<std::vector<int32_t>> belief_revise_KB(const std::vector<std::vector<int32_t>>& beliefs, const std::vector<std::vector<int32_t>>& revision_formula) noexcept;
unsigned long hamming(
        const std::bitset<512>& state, const std::vector<std::bitset<512>>& belief_set) noexcept;
unsigned long state_difference(
        const std::vector<bool>& state, const std::vector<std::vector<bool>>& belief_set);

#endif
