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
#ifndef FILE_H
#define FILE_H

#include <vector>
#include <cstdint>
#include <utility>
#include <fstream>
#include <variant>
#include <unordered_map>

enum class type_format {
    CNF,
    DNF,
    RAW
};

//Returns a vector of data with a enum saying what type the variant holds
std::pair<type_format, std::variant<std::vector<std::vector<bool>>, std::vector<std::vector<int32_t>>>> read_file(const char *path) noexcept;

std::unordered_map<int32_t, unsigned long> read_pd_ordering(const char *path) noexcept;

#endif
