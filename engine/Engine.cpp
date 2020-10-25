#include "Engine.h"

void Engine::Init()
{
    if(mIsInited)
    {
        mLogger.LogError("Error in Engine::Init() : Engine already has been initialized.");
        return;
    }

    mLogger.LogEvent("Engine Init");
    mTimer.Init();

    mTimer.RegisterTick(mFPSCalculatingTick);
    mTimer.RegisterTick(mEngineUpdateTick);

    // Init Code here

    mIsInited = true;
}

void Engine::Update()
{
    mTimer.Update();

    /* Calculating FPS in every mFPSCalculatingTerm */
    if(mTimer.GetDeltaTick(mFPSCalculatingTick) >= mFPSCalculatingTerm)
    {
        mTimer.UpdateTick(mFPSCalculatingTick);
        mCurrentFPS = mSumFPS / mFrameCount;
        mSumFPS = 0.0;
        mFrameCount = 0;
        mLogger.LogTrace("Current FPS : " + std::to_string(mCurrentFPS));
    }

    /* Update Engine */
    if((mTargetFPS == 0) || (mTimer.GetDeltaTick(mEngineUpdateTick) >= (1.0 / mTargetFPS)))
    {
        // Update code here

        mFrameCount++;
        mSumFPS += 1.0 / mTimer.GetDeltaTick(mEngineUpdateTick);
        mTimer.UpdateTick(mEngineUpdateTick);
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

Engine::Engine() : mLogger(Logger::LogLevel::Trace, true), mTimer() {}