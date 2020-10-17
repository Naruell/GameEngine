#pragma once
#include <string>

#include "Timer.h"
#include "Logger.h"

class Engine
{
public:
    static Engine& Instance() { static Engine instance; return instance; }
    static Timer& GetTimer() { return Instance().mTimer; }
    static Logger& GetLogger() { return Instance().mLogger; }

    void Init() const;
    void Update();
    void Shutdown();

    [[nodiscard]] double GetTargetFPS() const noexcept;
    void                 SetTargetFPS(double) noexcept;
    [[nodiscard]] double GetCurrentFPS() const noexcept;

private:
    Engine();

    Timer mTimer;
    Logger mLogger;

private:
    // FPS, delta time
    double            mTargetFPS = 30; // 0 for infinite frame lock
    const std::string mEngineUpdateTick = "EngineUpdateTick";

    const std::string mFPSCalculatingTick = "FPSCalculatingTick";
    double            mFPSCalculatingTerm = 2.0;
    int               mFrameCount = 0;
    double            mSumFPS = 0;
    double            mCurrentFPS = 0;
};