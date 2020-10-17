#pragma once
#include <string>
#include <fstream>

class Logger
{
public:
    enum class LogLevel
    {
        Trace,
        Debug,
        Event,
        Warning,
        Error,
        Count
    };

    Logger();
    explicit Logger(LogLevel minLevel, bool isUsingConsole);

    void LogTrace(const std::string& message) { Log(LogLevel::Trace, message); };
    void LogDebug(const std::string& message) { Log(LogLevel::Debug, message); };
    void LogEvent(const std::string& message) { Log(LogLevel::Event, message); };
    void LogWarning(const std::string& message) { Log(LogLevel::Warning, message); };
    void LogError(const std::string& message) { Log(LogLevel::Error, message); };

private:
    void Log(LogLevel, const std::string&);
    void LogStream(std::ostream&, LogLevel, const std::string&);

    std::ofstream mOutStream;
    LogLevel mMinLevel;
    bool mIsUsingConsole;
    int mLogIndex;
    int mLogCount[static_cast<int>(LogLevel::Count)] = {};
};