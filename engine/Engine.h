// Todo : prevent the case that user tries to call Engine instance before it is initialized
// Cur  : Added mIsInited flag variable.

#pragma once
#include <string>

#include "Timer.h"
#include "Logger.h"
#include "GameState.h"
#include "Loader.h"

class Engine
{
public:
    static Engine& Instance() { static Engine instance; return instance; }
    static Timer&  GetTimer() { return Instance().mTimer; }
    static Logger& GetLogger() { return Instance().mLogger; }
    static Loader& GetLoader() { return Instance().mLoader; }

    void Init();
    void Update();
    void Shutdown();

    [[nodiscard]] double GetTargetFPS() const noexcept;
    void                 SetTargetFPS(double newTargetFPS) noexcept;
    [[nodiscard]] double GetCurrentFPS() const noexcept;

private:
    Engine();

    Timer  mTimer;
    Logger mLogger;
    Loader mLoader;

private:
    bool mIsInited = false;

    // FPS, delta time
    double            mTargetFPS = 30.0; // 0 for infinite frame lock
    const std::string mEngineUpdateTick = "EngineUpdateTick";

    const std::string mFPSCalculatingTick = "FPSCalculatingTick";
    double            mFPSCalculatingTerm = 2.0;
    int               mFrameCount = 0;
    double            mSumFPS = 0.0;
    double            mCurrentFPS = 0.0;
};