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
