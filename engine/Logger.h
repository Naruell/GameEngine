// Todo : solve LogLevel associated issues (hard coding)

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
    void PrintLogCount();

private:
    void Log(LogLevel logLevel, const std::string& message);
    void LogStream(std::ostream& outStream, LogLevel logLevel, const std::string& message) const;

    std::ofstream mOutStream;
    LogLevel      mMinLevel;
    bool          mIsUsingConsole;
    int           mLogIndex;
    int           mLogCount[static_cast<int>(LogLevel::Count)] = {};
};