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

#endif
