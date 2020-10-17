#include "Engine.h"

int main()
{
    Engine::Instance().Init();
    Engine::GetLogger().LogDebug("Hello! world!");
    Engine::Instance().SetTargetFPS(60);

    Engine::GetTimer().RegisterTick("Hi");
    Engine::GetLogger().LogDebug("Tick for \"Hi\" : " + std::to_string(Engine::GetTimer().GetDeltaTick("Hi")));
    Engine::Instance().Update();
    Engine::GetLogger().LogDebug("Tick for \"Hi\" : " + std::to_string(Engine::GetTimer().GetDeltaTick("Hi")));

    while(true)
    {
        Engine::Instance().Update();
    }

    Engine::Instance().Shutdown();
    return 0;
}