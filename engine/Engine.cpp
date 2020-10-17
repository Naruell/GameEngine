#include "Engine.h"

void Engine::Init() const
{
    GetLogger().LogEvent("Engine Init");
    GetTimer().Init();

    GetTimer().RegisterTick(mFPSCalculatingTick);
    GetTimer().RegisterTick(mEngineUpdateTick);

    // Init Code here
}

void Engine::Update()
{
    GetTimer().Update();

    /* Calculating FPS in every mFPSCalculatingTerm */
    if(GetTimer().GetDeltaTick(mFPSCalculatingTick) >= mFPSCalculatingTerm)
    {
        GetTimer().UpdateTick(mFPSCalculatingTick);
        mCurrentFPS = mSumFPS / mFrameCount;
        mSumFPS = 0.0;
        mFrameCount = 0;
        GetLogger().LogTrace("Current FPS : " + std::to_string(mCurrentFPS));
    }

    /* Update Engine */
    if((mTargetFPS == 0) || GetTimer().GetDeltaTick(mEngineUpdateTick) >= 1 / mTargetFPS)
    {
        // Update code here

        mFrameCount++;
        mSumFPS += 1 / GetTimer().GetDeltaTick(mEngineUpdateTick);
        GetTimer().UpdateTick(mEngineUpdateTick);
    }
}

void Engine::Shutdown()
{

}

double Engine::GetTargetFPS() const noexcept
{
    return mTargetFPS;
}

void Engine::SetTargetFPS(double newTargetFPS) noexcept
{
    mTargetFPS = newTargetFPS;
}

double Engine::GetCurrentFPS() const noexcept
{
    return mCurrentFPS;
}

Engine::Engine() : mLogger(Logger::LogLevel::Trace, true) {}