#include "Timer.h"
#include "Engine.h"

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
    auto itr = mTicks.find(tickName);
    if(itr == mTicks.end())
    {
        Engine::GetLogger().LogError("Error in Timer::UpdateTick() : Couldn't find pair from the key \"" + tickName + "\".");
    }
    else
    {
        itr->second = std::chrono::system_clock::now();
    }
}

double Timer::GetDeltaTick(const std::string& tickName) const
{
    auto itr = mTicks.find(tickName);
    if(itr == mTicks.end())
    {
        Engine::GetLogger().LogError("Error in Timer::GetDeltaTick() : Couldn't find pair from the key \"" + tickName + "\".");
        return 0;
    }
    else
    {
        return std::chrono::duration<double>(mLastTick - itr->second).count();
    }
}

double Timer::GetDeltaTime() const noexcept
{
    return mDeltaTime;
}

double Timer::GetElapsedTime() const noexcept
{
    return mElapsedTime;
}
