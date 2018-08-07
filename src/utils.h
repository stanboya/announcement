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
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdint>

std::vector<std::vector<bool>> convert_to_bool(const std::vector<std::vector<int32_t>>& state) noexcept;
std::vector<std::vector<int32_t>> convert_to_num(const std::vector<std::vector<bool>>& state) noexcept;

void print_formula_dnf(const std::vector<std::vector<int32_t>>& formula) noexcept;

//Converts between CNF and DNF using the distributive property, hence why this function can be shared
std::vector<std::vector<int32_t>> convert_normal_forms(const std::vector<std::vector<int32_t>>& normal_clauses) noexcept;

//Converts between bool vectors and DNF vectors
std::vector<std::vector<int32_t>> convert_raw(const std::vector<std::vector<bool>>& input_bits) noexcept;

//Converts between DNF vectors and bool vectors
std::vector<std::vector<bool>> convert_dnf_to_raw(const std::vector<std::vector<int32_t>>& clause_list) noexcept;

void simplify_dnf(std::vector<std::vector<int32_t>>& formula) noexcept;

#endif
