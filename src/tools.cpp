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
#include <vector>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iterator>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tools.h"
#include "utils.h"
#include "belief.h"

bool sat(const std::vector<std::vector<int32_t>>& clause_list) noexcept {
    const char *input_filename = ".tmp.input";
    const char *output_filename = ".tmp.output";

    if (access(input_filename, F_OK) == -1) {
        creat(input_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }
    if (access(output_filename, F_OK) == -1) {
        creat(output_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }

    {
        std::ofstream ofs{input_filename, std::ios_base::out | std::ios_base::trunc};
        if (!ofs) {
            std::cerr << "Unable to open output file\n";
            exit(EXIT_FAILURE);
        }

        for (const auto& clause : clause_list) {
            std::copy(clause.cbegin(), clause.cend(), std::ostream_iterator<int32_t>(ofs, " "));
            ofs << "0\n";
        }
    }

    bool trying_parallel = true;

retry:
    if (trying_parallel) {
        system("./sat/parallel/glucose-syrup_release .tmp.input 2> /dev/null | grep -o 's [SU]' > .tmp.output");
    } else {
        system("./sat/simp/glucose_release .tmp.input 2> /dev/null | grep -o 's [SU]' > .tmp.output");
    }

    std::ifstream ifs{output_filename};
    if (!ifs) {
        std::cerr << "Unable to open results file\n";
        exit(EXIT_FAILURE);
    }

    for (std::string line; std::getline(ifs, line);) {
        if (line.empty()) {
            continue;
        }
        //Line is in expected format
        if (line.front() == 's' && line.size() >= 3) {
            switch(line[2]) {
                case 'S':
                    return true;
                case 'U':
                    return false;
                default:
                    std::cerr << "SAT solver returned unknown status\n";
                    exit(EXIT_FAILURE);
            }
        }
    }
    if (trying_parallel) {
        trying_parallel = false;
        goto retry;
    }
    std::cerr << "SAT solver did not return proper output format\n";
    exit(EXIT_FAILURE);
}

std::vector<std::vector<int32_t>> allsat(const std::vector<std::vector<int32_t>>& clause_list) noexcept {
    const char *input_filename = ".tmp.input";
    const char *output_filename = ".tmp.output";

    if (access(input_filename, F_OK) == -1) {
        creat(input_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }
    if (access(output_filename, F_OK) == -1) {
        creat(output_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }

    {
        std::ofstream ofs{input_filename, std::ios_base::out | std::ios_base::trunc};
        if (!ofs) {
            std::cerr << "Unable to open output file\n";
            exit(EXIT_FAILURE);
        }

        for (const auto& clause : clause_list) {
            std::copy(clause.cbegin(), clause.cend(), std::ostream_iterator<int32_t>(ofs, " "));
            ofs << "0\n";
        }
    }

    system("./minisat_all/bdd_minisat_all_release .tmp.input .tmp.output");

    std::ifstream ifs{output_filename};
    if (!ifs) {
        std::cerr << "Unable to open results file\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::vector<int32_t>> output_states;

    for (std::string line; std::getline(ifs, line);) {
        std::istringstream iss{std::move(line)};

        std::vector<int32_t> clause_tokens;

        clause_tokens.assign(std::istream_iterator<int32_t>(iss),
                std::istream_iterator<int32_t>());
        clause_tokens.erase(std::remove(clause_tokens.begin(), clause_tokens.end(), 0),
                clause_tokens.end());
        clause_tokens.shrink_to_fit();

        if (clause_tokens.empty()) {
            continue;
        }

        output_states.emplace_back(std::move(clause_tokens));
    }

    return output_states;
}

std::vector<std::vector<int32_t>> belief_revise(const std::vector<std::vector<int32_t>>& beliefs, const std::vector<std::vector<int32_t>>& revision_formula) noexcept {
#if 1
    //Formula must be CNF
    //Beliefs must be bit vectors

    auto bel = convert_dnf_to_raw(beliefs);

    assert(revision_formula.size() == 1);

    //This is just needed to convert dnf formula into cnf input for belief revision
    std::vector<std::vector<int32_t>> converted_form;
    for (const auto clause : revision_formula.front()) {
        std::vector<int32_t> cnf_clause;
        cnf_clause.emplace_back(clause);
        converted_form.emplace_back(std::move(cnf_clause));
    }

    return revise_beliefs(bel, converted_form, {}, nullptr);

#else
    const char *input_filename = ".tmp.input";
    const char *output_filename = ".tmp.output";

    if (access(input_filename, F_OK) == -1) {
        creat(input_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }
    if (access(output_filename, F_OK) == -1) {
        creat(output_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }

    {
        std::ofstream ofs{input_filename, std::ios_base::out | std::ios_base::trunc};
        if (!ofs) {
            std::cerr << "Unable to open output file\n";
            exit(EXIT_FAILURE);
        }

        ofs << "p dnf\n";

        for (const auto& clause : beliefs) {
            std::copy(clause.cbegin(), clause.cend(), std::ostream_iterator<int32_t>(ofs, " "));
            ofs << "0\n";
        }
    }
    {
        std::ofstream ofs{output_filename, std::ios_base::out | std::ios_base::trunc};
        if (!ofs) {
            std::cerr << "Unable to open output file\n";
            exit(EXIT_FAILURE);
        }

        ofs << "p dnf\n";

        for (const auto& clause : revision_formula) {
            std::copy(clause.cbegin(), clause.cend(), std::ostream_iterator<int32_t>(ofs, " "));
            ofs << "0\n";
        }
    }

    system("./bin/belief_rev -b .tmp.input -f .tmp.output -o .tmp.2 > /dev/null 2>&1");

    std::ifstream ifs{".tmp.2"};
    if (!ifs) {
        std::cerr << "Unable to open results file\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::vector<int32_t>> output_states;

    for (std::string line; std::getline(ifs, line);) {
        std::istringstream iss{std::move(line)};

        std::vector<int32_t> clause_tokens;

        clause_tokens.assign(std::istream_iterator<int32_t>(iss),
                std::istream_iterator<int32_t>());
        clause_tokens.erase(std::remove(clause_tokens.begin(), clause_tokens.end(), 0),
                clause_tokens.end());
        clause_tokens.shrink_to_fit();

        if (clause_tokens.empty()) {
            continue;
        }

        output_states.emplace_back(std::move(clause_tokens));
    }

    return output_states;
#endif
}

