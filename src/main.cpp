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

#include "UI/mainwindow.h"
#include "announce.h"
#include "interactive.h"
#include "tools.h"

static struct option long_options[]
        = {{"verbose", no_argument, 0, 'v'}, {"output", required_argument, 0, 'o'}, {0, 0, 0, 0}};

#define print_help() \
    do { \
        printf("usage options:\n" \
               "\t [v]erbose               - Output in verbose mode\n" \
               "\t [o]utput                - File to output belief announcement to\n" \
               "\t [t]est                  - Run worst case scenario test and exit\n" \
               "\t [h]elp                  - this message\n"); \
    } while (0)

int main(int argc, char** argv) {
    const char* output_file = nullptr;
    verbose = false;
    int agent_count = 0;
    for (;;) {
        int c;
        int option_index = 0;
        if ((c = getopt_long(argc, argv, "hvo:t:", long_options, &option_index)) == -1) {
            break;
        }
        switch (c) {
            case 'v':
                verbose = true;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 't':
                agent_count = strtol(optarg, NULL, 10);
                if (agent_count > 64 || agent_count <= 1) {
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
    if (agent_count > 1) {
        std::vector<agent> agent_list;
        std::stringstream belief;
        for (int i = 0; i < agent_count; ++i) {
            belief << (i + 1);
            if (i < (agent_count - 1)) {
                belief << " and ";
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
        find_announcement(agent_list);
        return EXIT_SUCCESS;
    }

    QApplication app(argc, argv);

    MainWindow m{};
    m.show();

    app.exec();

    return EXIT_SUCCESS;
}
