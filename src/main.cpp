#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <variant>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <getopt.h>
#include "tools.h"
#include "announce.h"

static struct option long_options[] = {
    {"verbose",     no_argument,       0, 'v'},
    {"output",      required_argument, 0, 'o'},
    {0,             0,                 0,  0}
};

#define print_help() \
    do { \
        printf("usage options:\n"\
                "\t [v]erbose               - Output in verbose mode\n"\
                "\t [o]utput                - File to output revised beliefse to\n"\
                "\t [h]elp                  - this message\n"\
                );\
    } while(0)

int main(int argc, char **argv) {
    const char *output_file = nullptr;
    bool verbose = false;
    for (;;) {
        int c;
        int option_index = 0;
        if ((c = getopt_long(argc, argv, "hvo:", long_options, &option_index)) == -1) {
            break;
        }
        switch (c) {
            case 'v':
                verbose = true;
                break;
            case 'o':
                output_file = optarg;
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

    std::vector<std::vector<std::vector<int32_t>>> goals;

    std::vector<std::vector<int32_t>> goal_1;
    std::vector<std::vector<int32_t>> goal_2;

    goal_1.emplace_back(std::vector<int32_t>{1, 2, 3, 4});
    goal_1.emplace_back(std::vector<int32_t>{2, 3, 4, 5});
    goal_1.emplace_back(std::vector<int32_t>{4, 5, 6});

    goal_2.emplace_back(std::vector<int32_t>{7, 8});
    goal_2.emplace_back(std::vector<int32_t>{9});

    goals.emplace_back(goal_1);
    goals.emplace_back(goal_2);

    std::cout << goals_consistent(goals) << "\n";

    return EXIT_SUCCESS;
}
