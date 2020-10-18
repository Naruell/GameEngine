#include <iostream>
#include "Logger.h"
#include "Engine.h"

Logger::Logger() : Logger(Logger::LogLevel::Trace, true) {}
Logger::Logger(Logger::LogLevel minLevel, bool isUsingConsole)
    : mOutStream("Log.txt"), mMinLevel(minLevel), mIsUsingConsole(isUsingConsole), mLogIndex(0) {}

void Logger::PrintLogCount()
{
    std::string logCountInfo = "";
    logCountInfo += "Trace[" + std::to_string(mLogCount[static_cast<int>(LogLevel::Trace)]) + "], ";
    logCountInfo += "Debug[" + std::to_string(mLogCount[static_cast<int>(LogLevel::Debug)]) + "], ";
    logCountInfo += "Event[" + std::to_string(mLogCount[static_cast<int>(LogLevel::Event)]) + "], ";
    logCountInfo += "Warning[" + std::to_string(mLogCount[static_cast<int>(LogLevel::Warning)]) + "], ";
    logCountInfo += "Error[" + std::to_string(mLogCount[static_cast<int>(LogLevel::Error)]) + ']';
    Log(LogLevel::Debug, logCountInfo);
}

void Logger::Log(Logger::LogLevel logLevel, const std::string& message)
{
    LogStream(mOutStream, logLevel, message);

    if(mIsUsingConsole)
        LogStream(std::cout, logLevel, message);

    mLogCount[static_cast<int>(logLevel)]++;
    mLogIndex++;
}

void Logger::LogStream(std::ostream& outStream, Logger::LogLevel logLevel, const std::string& message) const
{
    if(logLevel >= mMinLevel)
    {
        outStream.precision(4);
        outStream << mLogIndex << "\t[" << std::fixed << Engine::GetTimer().GetElapsedTime() << "]\t";

        switch (logLevel)
        {
            case LogLevel::Trace:
                outStream << "Trace\t";
                break;
            case LogLevel::Debug:
                outStream << "Debug\t";
                break;
            case LogLevel::Event:
                outStream << "Event\t";
                break;
            case LogLevel::Warning:
                outStream << "Warning\t";
                break;
            case LogLevel::Error:
                outStream << "Error\t";
                break;
            default:
                break;
        }
        outStream << message << '\n';
    }
}