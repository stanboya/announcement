#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <cstdint>

bool sat(const std::vector<std::vector<int32_t>>& formula) noexcept;
std::vector<std::vector<int32_t>> allsat(const std::vector<std::vector<int32_t>>& formula) noexcept;
std::vector<std::vector<int32_t>> belief_revise(const std::vector<std::vector<int32_t>>& beliefs, const std::vector<std::vector<int32_t>>& revision_formula) noexcept;

#endif
