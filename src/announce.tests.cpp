#include <catch2/catch.hpp>
#include "announce.h"
#include "interactive.h"
#include "utils.h"
#include <iterator>
#include <sstream>
#include <iostream>
#include "stopwatch/stopwatch.hpp"

TEST_CASE("get_dnf_from_equation") {
        SECTION("basic test 1") {
                std::string belief_str = "not 1 or 2";

                std::stringstream ss{shunting_yard(belief_str)};
                std::vector<std::string> belief_tokens{
                std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};
                REQUIRE(print_formula_dnf(get_dnf_from_equation(belief_tokens)) == "(not 1 and not 2) or (not 1 and 2) or (1 and 2)\n");
        }

        SECTION("basic test 2") {
                std::string belief_str = "not 1 or not 2";

                std::stringstream ss{shunting_yard(belief_str)};
                std::vector<std::string> belief_tokens{
                std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};
                REQUIRE(print_formula_dnf(get_dnf_from_equation(belief_tokens)) == "(not 1 and not 2) or (1 and not 2) or (not 1 and 2)\n");
        }     
}

TEST_CASE("Basic Failing Test 1") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "not 1 or 2";
    goal1str = "2";
    belief2str = "not 1 or not 2";
    goal2str = "(not 2) and 1";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "No possible satisfying assignment was found\n");
    }
}

TEST_CASE("Basic Test 1") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "not 1 or 2";
    goal1str = "(not 1 and 2) or (1 and 2)";
    belief2str = "not 1 or not 2";
    goal2str = "not 1 and 2";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "(2)\n");
    }

    
}

TEST_CASE("Basic Test 2") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(not 1) or 2 or (3 and 1)";
    goal1str = "(2 and 3 and 1) or (1 and not 2 and 3)";
    belief2str = "(not 1) or ((not 2) and 3)";
    goal2str = "not 2 and 1 and 3";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    
    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "(1 and 3)\n");
    }
    
}


TEST_CASE("Basic Test 3") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(not 1) or 2 or 3";
    goal1str = "2 and 3";
    belief2str = "(not 1) or ((not 2) and 3)";
    goal2str = "(not 2) and 1 and 3";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "No possible satisfying assignment was found\n");
    }
    
}

TEST_CASE("Basic Test 4") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(not 1) or 2 or 3";
    goal1str = "2 and 3";
    belief2str = "(not 1) or ((not 2) and 3)";
    goal2str = "3";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "No possible satisfying assignment was found\n");
    }
    
}

TEST_CASE("Basic Test 5") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(not 1) or 2 or 3";
    goal1str = "2 and 3";
    belief2str = "(not 2) and 1 or 3";
    goal2str = "3 or 1";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "No possible satisfying assignment was found\n");
    }
    
}


TEST_CASE("Basic Test 6") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(not 1 and 3) or (not 2 and 1)";
    goal1str = "(1 and not 2 and not 3) or (1 and not 2 and 3)";
    belief2str = "(not 1 and 2 and not 3) or (not 1 and 2 and 3) or (1 and 2 and not 3) or (1 and 2 and 3)";
    goal2str = "(1 and 2 and not 3) or (1 and 2 and 3)";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "(1)\n");
    }
    
}

TEST_CASE("Basic Test 7") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "not 1 or 2";
    goal1str = "1 and 2";
    belief2str = "(not 1) or (not 2)";
    goal2str = "1 and not 2";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "(1)\n");
    }
    
}

TEST_CASE("Basic Test 8") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "not 1 or 2";
    goal1str = "not 1 or 2";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "(not 1) or (2)\n");
    }
    
}

TEST_CASE("Basic Failing Test 2") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(not 1) or 2";
    goal1str = "2 and (not 1)";
    belief2str = "(not 1) or (not 2)";
    goal2str = "(not 2) and 1";


    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "No possible satisfying assignment was found\n");
    }

//     SECTION("John's Implementation") {
//         REQUIRE(find_announcement(agents) == "No possible satisfying assignment was found\n");
//     }
}

TEST_CASE("Timed Test (1 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "1";
    goal1str = "1";
    belief2str = "1";
    goal2str = "1";




    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));
    
    SECTION("Konstantin's Implementation") {

        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(1)\n");
        }

        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";
    }
    
}

TEST_CASE("Timed Test (2 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(1 and 2)";
    goal1str = "1 and 2";
    belief2str = "(not 1 and 2) or 2";
    goal2str = "2";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(2)\n");
        }

        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";
    }
    
}

TEST_CASE("Timed Test (3 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(1 and 2 and 3)";
    goal1str = "(1 and 2 and 3)";
    belief2str = "(not 1 and 2 and 3) or 3";
    goal2str = "3";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(3)\n");
        }

        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";
    }
    
}

TEST_CASE("Timed Test (4 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(1 and 2 and 3 and 4)";
    goal1str = "(1 and 2 and 3 and 4)";
    belief2str = "(not 1 and 2 and 3 and 4) or 4";
    goal2str = "4";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(4)\n");
        }

        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";
    }
    
}


TEST_CASE("Timed Test (5 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(1 and 2 and 3 and 4 and 5)";
    goal1str = "(1 and 2 and 3 and 4 and 5)";
    belief2str = "(not 1 and 2 and 3 and 4 and 5) or 5";
    goal2str = "5";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(5)\n");
        }

        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";
    }
    
}

TEST_CASE("Timed Test (6 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(1 and 2 and 3 and 4 and 5 and 6)";
    goal1str = "(1 and 2 and 3 and 4 and 5 and 6)";
    belief2str = "(not 1 and 2 and 3 and 4 and 5 and 6) or 6";
    goal2str = "6";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(6)\n");
        }

        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";
    }
    
}

TEST_CASE("Timed Test (7 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7)";
    goal1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7)";
    belief2str = "(not 1 and 2 and 3 and 4 and 5 and 6 and 7) or 7";
    goal2str = "7";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(7)\n");
        }
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";

    }
    
}

// TEST_CASE("Timed Test (8 Variables)") {
//     std::string belief1str, belief2str, goal1str, goal2str;

//     belief1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7 and 8)";
//     goal1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7 and 8)";
//     belief2str = "(not 1 and 2 and 3 and 4 and 5 and 6 and 7 and 8) or 8";
//     goal2str = "8";



//     std::vector<agent> agents{};

//     std::stringstream ss{shunting_yard(belief1str)};
//     std::vector<std::string> belief1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal1str)};
//     std::vector<std::string> goal1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.push_back(create_agent(belief1_tokens, goal1_tokens));

//     ss = std::stringstream{shunting_yard(belief2str)};
//     std::vector<std::string> belief2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal2str)};
//     std::vector<std::string> goal2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

//     SECTION("Konstantin's Implementation") {
//         std::chrono::nanoseconds elapsed_ns;
//         {
//                 Stopwatch stopwatch{ elapsed_ns };
//                 REQUIRE(find_announcement_KB(agents) == "(8)\n");
//         }
//         const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
//         std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";

//     }
    
// }

// TEST_CASE("Timed Test (9 Variables)") {
//     std::string belief1str, belief2str, goal1str, goal2str;

//     belief1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7 and 8 and 9)";
//     goal1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7 and 8 and 9)";
//     belief2str = "(not 1 and 2 and 3 and 4 and 5 and 6 and 7 and 8 and 9) or 9";
//     goal2str = "9";



//     std::vector<agent> agents{};

//     std::stringstream ss{shunting_yard(belief1str)};
//     std::vector<std::string> belief1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal1str)};
//     std::vector<std::string> goal1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.push_back(create_agent(belief1_tokens, goal1_tokens));

//     ss = std::stringstream{shunting_yard(belief2str)};
//     std::vector<std::string> belief2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal2str)};
//     std::vector<std::string> goal2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

//     SECTION("Konstantin's Implementation") {
//         std::chrono::nanoseconds elapsed_ns;
//         {
//                 Stopwatch stopwatch{ elapsed_ns };
//                 REQUIRE(find_announcement_KB(agents) == "(9)\n");
//         }
//         const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
//         std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";

//     }
    
// }

TEST_CASE("Timed Test (10 Variables)") {
    std::string belief1str, belief2str, goal1str, goal2str;

    belief1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7 and 8 and 9 and 10)";
    goal1str = "(1 and 2 and 3 and 4 and 5 and 6 and 7 and 8 and 9 and 10)";
    belief2str = "(not 1 and 2 and 3 and 4 and 5 and 6 and 7 and 8 and 9 and 10) or 10";
    goal2str = "10";



    std::vector<agent> agents{};

    std::stringstream ss{shunting_yard(belief1str)};
    std::vector<std::string> belief1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal1str)};
    std::vector<std::string> goal1_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.push_back(create_agent(belief1_tokens, goal1_tokens));

    ss = std::stringstream{shunting_yard(belief2str)};
    std::vector<std::string> belief2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    ss = std::stringstream{shunting_yard(goal2str)};
    std::vector<std::string> goal2_tokens{
            std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

    agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

    SECTION("Konstantin's Implementation") {
        std::chrono::nanoseconds elapsed_ns;
        {
                Stopwatch stopwatch{ elapsed_ns };
                REQUIRE(find_announcement_KB(agents) == "(10)\n");
        }
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_ns).count();
        std::cout << "Time Taken: " << elapsed_ms << "ms" << "\n";

    }
    
}


