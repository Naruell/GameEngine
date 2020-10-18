#include <cassert>
#include <functional>

#include "Engine.h"

int main()
{
    Engine::Instance().Init();
    /*
    Engine::GetLogger().LogDebug("Hello! world!");
    Engine::Instance().SetTargetFPS(60);

    Engine::GetTimer().RegisterTick("Hi");
    Engine::GetLogger().LogDebug("Tick for \"Hi\" : " + std::to_string(Engine::GetTimer().GetDeltaTick("Hi")));
    Engine::Instance().Update();
    Engine::GetLogger().LogDebug("Tick for \"Hi\" : " + std::to_string(Engine::GetTimer().GetDeltaTick("Hi")));
    */


    /* Test Engine Reference returning function */
    std::function<Engine& ()> get_engine_func =
            []() -> Engine&
            {
                Engine& instance = Engine::Instance();
                return instance;
            };

    Engine& engine = get_engine_func();
    assert( &engine == &Engine::Instance() );

    if( &engine == &Engine::Instance() )
        Engine::GetLogger().LogDebug("&engine and &Engine::Instance() are SAME");
    else
        Engine::GetLogger().LogError("&engine and &Engine::Instance() are NOT SAME");


    /* Test Timer Reference returning function */
    std::function<Timer& ()> get_timer_func =
            []() -> Timer&
            {
                Timer& instance = Engine::GetTimer();
                return instance;
            };

    Timer& timer = get_timer_func();
    assert( &timer == &Engine::GetTimer() );

    if( &timer == &Engine::GetTimer() )
        Engine::GetLogger().LogDebug("&timer and &Engine::GetTimer() are SAME");
    else
        Engine::GetLogger().LogError("&timer and &Engine::GetTimer() are NOT SAME");


    /* Test Logger Reference returning function */
    std::function<Logger& ()> get_logger_func =
            []() -> Logger&
            {
                Logger& instance = Engine::GetLogger();
                return instance;
            };

    Logger& logger = get_logger_func();
    assert( &logger == &Engine::GetLogger() );

    if( &logger == &Engine::GetLogger() )
        Engine::GetLogger().LogDebug("&logger and &Engine::GetLogger() are SAME");
    else
        Engine::GetLogger().LogError("&logger and &Engine::GetLogger() are NOT SAME");

    /*
    while(Engine::GetTimer().GetElapsedTime() <= 10)
    {
        Engine::Instance().Update();
    }

    Engine::GetLogger().PrintLogCount();
     */
    Engine::Instance().Shutdown();

    return 0;
}