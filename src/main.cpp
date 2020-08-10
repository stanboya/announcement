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
#include <QApplication>
#include <QPushButton>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <variant>
#include <chrono>
#include "stopwatch/stopwatch.hpp"

#include "UI/mainwindow.h"
#include "announce.h"
#include "interactive.h"
#include "tools.h"

static struct option long_options[]
        = {{"verbose", no_argument, 0, 'v'}, {"test", required_argument, 0, 't'},
                {"brute", no_argument, 0, 'b'}, {"help", no_argument, 0, 'h'}, {0, 0, 0, 0}};

#define print_help() \
    do { \
        printf("usage options:\n" \
               "\t [v]erbose               - Output in verbose mode\n" \
               "\t [t]est                  - Run worst case scenario test and exit\n" \
               "\t [b]rute                 - Brute force all 2 variable combinations\n" \
               "\t [h]elp                  - this message\n"); \
    } while (0)

int main(int argc, char** argv) {
    verbose = false;
    int agent_count = 0;
    bool brute_force = false;
    for (;;) {
        int c;
        int option_index = 0;
        if ((c = getopt_long(argc, argv, "hvt:b", long_options, &option_index)) == -1) {
            break;
        }
        switch (c) {
            case 'v':
                verbose = true;
                break;
            case 'b':
                brute_force = true;
                break;
            case 't':
                agent_count = strtol(optarg, NULL, 10);
                if (agent_count > 64 || agent_count < 1) {
                    agent_count = 0;
                }
                break;
            case 'h':
                [[fallthrough]];
            case '?':
                [[fallthrough]];
            default:
                print_help();
                return EXIT_SUCCESS;
        }
    }
    if (brute_force) {
        std::vector<std::string> input_states;
        input_states.emplace_back("1 and 2");
        input_states.emplace_back("1 or 2");
        input_states.emplace_back("1 and not 2");
        input_states.emplace_back("1 or not 2");
        input_states.emplace_back("not 1 and 2");
        input_states.emplace_back("not 1 or 2");
        input_states.emplace_back("not 1 and not 2");
        input_states.emplace_back("not 1 or not 2");

        std::vector<std::vector<agent>> brute_input;

        for (int i = 0; i < 4096; ++i) {
            const auto idx1 = i % 8;
            const auto idx2 = (i / 8) % 8;
            const auto idx3 = (i / 64) % 8;
            const auto idx4 = (i / 64) / 8;

            std::vector<agent> agent_list;

            std::stringstream ss{shunting_yard(input_states[idx1])};
            std::vector<std::string> belief_tokens{
                    std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

            ss = std::stringstream{shunting_yard(input_states[idx2])};
            std::vector<std::string> goal_tokens{
                    std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

            agent_list.emplace_back(create_agent(belief_tokens, goal_tokens));

            ss = std::stringstream{shunting_yard(input_states[idx3])};
            belief_tokens = std::vector<std::string>{
                    std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

            ss = std::stringstream{shunting_yard(input_states[idx4])};
            goal_tokens = std::vector<std::string>{
                    std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

            agent_list.emplace_back(create_agent(belief_tokens, goal_tokens));

            brute_input.emplace_back(std::move(agent_list));
        }

        for (const auto& input : brute_input) {
            find_announcement_KB(input);
        }
        return EXIT_SUCCESS;
    }
    if (agent_count > 0) {
        std::vector<agent> agent_list;
        std::stringstream belief;
        for (int i = 0; i < agent_count; ++i) {
            belief << (i + 1);
            if (i < (agent_count - 1)) {
                belief << " or ";
            }
        }
        std::stringstream ss{shunting_yard(belief.str())};
        std::vector<std::string> belief_tokens{
                std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};
        const auto agent_belief = get_dnf_from_equation(belief_tokens);
        for (int i = 0; i < agent_count; ++i) {
            std::stringstream goal;

            if (i & 1) {
                goal << "not ";
            }
            goal << 1;

            std::string goal_string = goal.str();

            ss = std::stringstream{shunting_yard(goal_string)};
            std::vector<std::string> goal_tokens{
                    std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

            agent_list.push_back({agent_belief, get_dnf_from_equation(goal_tokens)});
        }
        std::cout << std::endl;
        std::chrono::nanoseconds elapsed_ns;
        {
            Stopwatch stopwatch{ elapsed_ns };
            std::cout << find_announcement_KB(agent_list);
        }

        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";
        
        return EXIT_SUCCESS;
    }

    QApplication app(argc, argv);

    MainWindow m{};
    m.show();

    app.exec();

    return EXIT_SUCCESS;
}
