#include "Timer.h"

void Timer::Init()
{
    mLastTick = std::chrono::system_clock::now();
}

void Timer::Update()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    mDeltaTime = std::chrono::duration<double>(now - mLastTick).count();
    mLastTick = now;
    mElapsedTime += mDeltaTime;
}

void Timer::RegisterTick(const std::string& tickName)
{
    mTicks.insert({tickName, mLastTick});
}

void Timer::UpdateTick(const std::string& tickName)
{
    mTicks.find(tickName)->second = std::chrono::system_clock::now();
}

double Timer::GetDeltaTick(const std::string& tickName) const
{
    return std::chrono::duration<double>(mLastTick - mTicks.find(tickName)->second).count();
}

double Timer::GetDeltaTime() const noexcept
{
    return mDeltaTime;
}

double Timer::GetElapsedTime() const noexcept
{
    return mElapsedTime;
}
