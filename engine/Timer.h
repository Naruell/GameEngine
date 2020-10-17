// Todo : solve string compare issue in mTicks

#pragma once
#include <chrono>
#include <string>
#include <unordered_map>

class Timer
{
public:
    Timer() = default;

    void Init();
    void Update();

    void RegisterTick(const std::string&);
    void UpdateTick(const std::string&);
    [[nodiscard]] double GetDeltaTick(const std::string&) const;

    [[nodiscard]] double GetDeltaTime() const noexcept;
    [[nodiscard]] double GetElapsedTime() const noexcept;

private:
    std::chrono::system_clock::time_point mLastTick;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> mTicks;

    double mDeltaTime = 0;
    double mElapsedTime = 0;
};