#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "Engine.h"


TEST_CASE( "Returning Reference of Engine / Components" )
{
    SECTION("Engine")
    {
        std::function<Engine& ()> get_engine_func =
                []() -> Engine&
                {
                    Engine& instance = Engine::Instance();
                    return instance;
                };

        Engine& engine = get_engine_func();
        REQUIRE( &engine == &Engine::Instance() );
    }


    SECTION("Timer")
    {
        std::function<Timer& ()> get_timer_func =
                []() -> Timer&
                {
                    Timer& instance = Engine::GetTimer();
                    return instance;
                };

        Timer& timer = get_timer_func();
        REQUIRE( &timer == &Engine::GetTimer() );
    }


    SECTION("Logger")
    {
        std::function<Logger& ()> get_logger_func =
                []() -> Logger&
                {
                    auto & instance = Engine::GetLogger();
                    return instance;
                };

        Logger& logger = get_logger_func();
        logger.LogError("HELLL");
        logger.LogDebug("DEBUG");
        REQUIRE( &logger == &Engine::GetLogger() );
    }

    SECTION("TESTER")
    {

    }
}