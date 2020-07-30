#include <catch2/catch.hpp>
#include "announce.h"
#include "interactive.h"
#include "utils.h"
#include <iterator>
#include <sstream>
#include <iostream>


// TEST_CASE("get_dnf_from_equation") {
//         SECTION("basic test") {
//                 std::string belief_str = "not 1 or 2";

//                 std::stringstream ss{shunting_yard(belief_str)};
//                 std::vector<std::string> belief_tokens{
//                 std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};
//                 REQUIRE(print_formula_dnf(get_dnf_from_equation(belief_tokens)) == "(not 1 and not 2) or (not 1 and 2) or (1 and 2)\n");
//         }

//         SECTION("basic test2") {
//                 std::string belief_str = "not 1 or not 2";

//                 std::stringstream ss{shunting_yard(belief_str)};
//                 std::vector<std::string> belief_tokens{
//                 std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};
//                 REQUIRE(print_formula_dnf(get_dnf_from_equation(belief_tokens)) == "(not 1 and not 2) or (1 and not 2) or (not 1 and 2)\n");
//         }
        

        
// }


TEST_CASE("Basic Test") {
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
        
    // SECTION("John's Implementation") {
    //     REQUIRE(find_announcement(agents) == "(1)\n");
    // }
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "No possible satisfying assignment was found\n");
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

    // SECTION("John's Implementation") {
    //     REQUIRE(find_announcement(agents) == "(1 and 3)\n");
    // }
    
    SECTION("Konstantin's Implementation") {
        REQUIRE(find_announcement_KB(agents) == "(1 and 3)\n");
    }
    
}


// TEST_CASE("Basic Test 3") {
//     std::string belief1str, belief2str, goal1str, goal2str;

//     belief1str = "(not 1) or 2 or 3";
//     goal1str = "2 and 3";
//     belief2str = "(not 1) or ((not 2) and 3)";
//     goal2str = "(not 2) and 1 and 3";


//     std::vector<agent> agents{};

//     std::stringstream ss{shunting_yard(belief1str)};
//     std::vector<std::string> belief1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal1str)};
//     std::vector<std::string> goal1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

//     ss = std::stringstream{shunting_yard(belief2str)};
//     std::vector<std::string> belief2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal2str)};
//     std::vector<std::string> goal2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

//     // SECTION("John's Implementation") {
//     //     REQUIRE(find_announcement(agents) == "(1 and 3)\n");
//     // }
    
//     SECTION("Konstantin's Implementation") {
//         REQUIRE(find_announcement_KB(agents) == "(1 and 3)\n");
//     }
    
// }

// TEST_CASE("Basic Test 4") {
//     std::string belief1str, belief2str, goal1str, goal2str;

//     belief1str = "(not 1) or 2 or 3";
//     goal1str = "2 and 3";
//     belief2str = "(not 1) or ((not 2) and 3)";
//     goal2str = "(not 2) and 1 or 3";


//     std::vector<agent> agents{};

//     std::stringstream ss{shunting_yard(belief1str)};
//     std::vector<std::string> belief1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal1str)};
//     std::vector<std::string> goal1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

//     ss = std::stringstream{shunting_yard(belief2str)};
//     std::vector<std::string> belief2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal2str)};
//     std::vector<std::string> goal2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

//     // SECTION("John's Implementation") {
//     //     REQUIRE(find_announcement(agents) == "(1 and 3)\n");
//     // }
    
//     SECTION("Konstantin's Implementation") {
//         REQUIRE(find_announcement_KB(agents) == "(1 and 3)\n");
//     }
    
// }

// TEST_CASE("Basic Failing Test") {
//     std::string belief1str, belief2str, goal1str, goal2str;

//     belief1str = "(not 1) or 2";
//     goal1str = "2 and (not 1)";
//     belief2str = "(not 1) or (not 2)";
//     goal2str = "(not 2) and 1";


//     std::vector<agent> agents{};

//     std::stringstream ss{shunting_yard(belief1str)};
//     std::vector<std::string> belief1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal1str)};
//     std::vector<std::string> goal1_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief1_tokens, goal1_tokens));

//     ss = std::stringstream{shunting_yard(belief2str)};
//     std::vector<std::string> belief2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     ss = std::stringstream{shunting_yard(goal2str)};
//     std::vector<std::string> goal2_tokens{
//             std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

//     agents.emplace_back(create_agent(belief2_tokens, goal2_tokens));

//     // SECTION("John's Implementation") {
//     //     REQUIRE(find_announcement(agents) == "No possible satisfying assignment was found\n");
//     // }
    
//     SECTION("Konstantin's Implementation") {
//         REQUIRE(find_announcement_KB(agents) == "No possible satisfying assignment was found\n");
//     }
// }

