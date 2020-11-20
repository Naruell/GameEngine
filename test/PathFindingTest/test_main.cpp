#define CATCH_CONFIG_MAIN
#include <catch.hpp>

/* Engine Codes */
#include "Engine.h"

/* Game Codes */
#include "AI.h"

#include <vector>
#include <iomanip>

#define START 10
#define GOAL 11
#define PATH 12

TEST_CASE("PathFinding")
{
    SECTION("Test Init")
    {
        Engine::Instance().Init();
    }

    SECTION("Warning Test")
    {
        int width = 7;
        int height = 4;
        std::vector<short> fieldVector =
        {
                1, 1, 0, 1, 1, 1, 1,
                1, 1, 0, 1, 0, 0, 1,
                0, 1, 1, 1, 0, 1, 1,
                0, 0, 0, 0, 1, 1, 1
        };

        PathFinding::Field field(width, height , fieldVector);

        PathFinding::Node start({0, 0}, 0);
        PathFinding::Node end({5, 3}, 0);


        PathFinding::Module testModule(start, end, field);

        REQUIRE(testModule.Run() == true);

        std::cout << "Path length : " << testModule.GetPath().size() << std::endl;
        for(PathFinding::Node node : testModule.GetPath())
        {
            fieldVector[node.GetLocation().x + node.GetLocation().y * width] = PATH;
        }

        fieldVector[start.GetLocation().x + start.GetLocation().y * width] = START;
        fieldVector[end.GetLocation().x + end.GetLocation().y * width] = GOAL;

        std::cout << "-----------------" << std::endl;
        for(int i = 0; i < fieldVector.size(); ++i)
        {
            if(i % width == 0) { std::cout << "| ";}
            char c;

            if(fieldVector[i] == 1) { c = ' '; }
            else if(fieldVector[i] == 0) { c = 'I'; }
            else if(fieldVector[i] == START) { c = 'S'; }
            else if(fieldVector[i] == GOAL) { c = 'G'; }
            else if(fieldVector[i] == PATH) { c = '.'; }
            else { c = ' '; }

            std::cout << c << ' ';
            if(i % width ==  width - 1) { std::cout << '|' << std::endl; }
        }
        std::cout << "-----------------" << std::endl;
    }
}