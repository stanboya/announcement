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
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <iterator>
#include <utility>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <cassert>
#include <variant>
#include <bitset>
#include <unordered_set>
#include "file.h"

//Returns an enum along with a variant, where the enum says what type the variant has
//This means this same piece of code can parse all 3 accepted input formats without needing seperate functions
std::pair<type_format, std::variant<std::vector<std::vector<bool>>, std::vector<std::vector<int32_t>>>> read_file(const char *path) noexcept {
    std::ifstream file{path};

    if (!file) {
        std::cerr << "Unable to open file " << strerror(errno) << "\n";
        std::cerr << path << "\n";
        return {};
    }

    std::vector<std::vector<int32_t>> output_clauses;
    std::vector<std::vector<bool>> output_bits;

    bool problem_found = false;
    type_format input_type = type_format::RAW;

    for (std::string line; std::getline(file, line);) {
        //Ignore empty lines
        if (line.empty()) {
            continue;
        }
        //Ignore comment lines
        if (line.front() == 'c') {
            continue;
        }
        //Parse problem line
        if (line.front() == 'p') {
            std::istringstream iss{std::move(line)};
            std::string dummy;
            std::string format;

            //Ignore the p token for the problem line
            iss >> dummy;
            iss >> format;

            if (format.find("cnf") != std::string::npos) {
                input_type = type_format::CNF;
            } else if (format.find("dnf") != std::string::npos) {
                input_type = type_format::DNF;
            } else if (format.find("raw") != std::string::npos) {
                input_type = type_format::RAW;
            } else {
                //Unknown data format
                std::cerr << "Unknown data format\n";
                return {};
            }
            problem_found = true;
            continue;
        }
        //The first non-comment line is not a problem statement, return error
        if (!problem_found) {
            std::cerr << "First non-comment line was not a problem statement\n";
            return {};
        }
        switch(input_type) {
            case type_format::CNF:
                [[fallthrough]];
            case type_format::DNF:
                {
                    std::vector<int32_t> clause_tokens;

                    std::istringstream iss{std::move(line)};

                    clause_tokens.assign(std::istream_iterator<int32_t>(iss),
                            std::istream_iterator<int32_t>());
                    clause_tokens.erase(std::remove(clause_tokens.begin(), clause_tokens.end(), 0),
                            clause_tokens.end());
                    clause_tokens.shrink_to_fit();

                    if (clause_tokens.empty()) {
                        continue;
                    }
                    output_clauses.emplace_back(std::move(clause_tokens));
                }
                break;
            case type_format::RAW:
                {
                    unsigned char c;
                    std::vector<bool> state;

                    std::istringstream iss{std::move(line)};

                    for (;;) {
                        iss >> c;
                        if (iss.bad() || (!isxdigit(c) && !iscntrl(c) && !isspace(c))) {
                            std::cerr << "Failed to parse hexadecimal state\n";
                            return {};
                        }
                        if (iscntrl(c) || isspace(c) || iss.eof()) {
                            break;
                        }

                        //Need to convert char for ternery to work
                        c = toupper(c);
                        int hex_value = (c >= 'A') ? (c - 'A' + 10) : (c - '0');

                        //Push the bit values in big-endian order
                        state.push_back(hex_value & 8);
                        state.push_back(hex_value & 4);
                        state.push_back(hex_value & 2);
                        state.push_back(hex_value & 1);
                    }

                    if (state.empty()) {
                        continue;
                    }
                    output_bits.emplace_back(std::move(state));
                }
                break;
            default:
                std::cerr << "Unknown data format enum value\n";
                return {};
        }
    }

    if (input_type == type_format::RAW) {
        if (output_bits.empty()) {
            std::cerr << "File must not be empty\n";
            exit(EXIT_FAILURE);
        }
        return {input_type, output_bits};
    } else {
        if (output_clauses.empty()) {
            std::cerr << "File must not be empty\n";
            exit(EXIT_FAILURE);
        }
        return {input_type, output_clauses};
    }
}
std::unordered_map<int32_t, unsigned long> read_pd_ordering(const char *path) noexcept {
    std::ifstream file{path};

    if (!file) {
        std::cerr << "Unable to open file " << strerror(errno) << "\n";
        std::cerr << path << "\n";
        return {};
    }

    unsigned long distance_value = 1;
    std::unordered_map<int32_t, unsigned long> orderings;
    int32_t max_variable = -1;

    for (std::string line; std::getline(file, line);) {
        //Ignore empty lines
        if (line.empty()) {
            continue;
        }

        for (const auto c : line) {
            if (!std::isdigit(c) && !std::isspace(c)) {
                std::cerr << "Line contained invalid character\n";
                return {};
            }
        }

        std::istringstream iss{std::move(line)};
        int32_t variable_num;

        while(iss >> variable_num) {
            if (variable_num <= 0) {
                std::cerr << "Variable numbers cannot be negative\n";
                return {};
            }
            orderings.emplace(variable_num, distance_value);
            max_variable = std::max(variable_num, max_variable);
        }
        ++distance_value;
    }

    for (auto i = 1; i < max_variable; ++i) {
        if (!orderings.count(i)) {
            std::cerr << "There must be no gaps in variable orderings\n";
            return {};
        }
    }

    for (auto& p : orderings) {
        p.second = max_variable - p.second;
    }

    return orderings;
}

