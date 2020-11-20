#define CATCH_CONFIG_MAIN
#include <catch.hpp>

/* Engine Codes */
#include "Engine.h"

/* Game Codes */
#include "MapEditor.h"

#include <string>

TEST_CASE("MapEditor")
{
    SECTION("General")
    {
        Engine::Instance().Init();

        MapEditor::MapEditor me;
        bool isLoaded = me.LoadLevel("map_test.json");

        REQUIRE(isLoaded == true);

        while(true)
        {
            me.Draw();
            me.Update();
        }
    }
}