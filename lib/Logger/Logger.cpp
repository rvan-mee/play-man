
#include "play-man/Logger/Logger.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>



Logger& Logger::GetInstance()
{
	static Logger logger;
	return logger;
}

constexpr std::string_view Logger::LogTypeToColoredString(LogType logType) const
{
	switch (logType)
	{
		case LogType::Error:
			return colorError + "[ERROR]: " + colorReset;
		case LogType::Warning:
			return COLOR_WARNING "[WARNING]: " + colorReset;
		case LogType::Info:
			return COLOR_INFO "[INFO]: " + colorReset;
		case LogType::Debug:
			return COLOR_DEBUG "[Debug]: " + colorReset;
		default:
			throw std::runtime_error("Tried to get string for unknown logtype: " + std::to_string(static_cast<int>(logType)));
	}
}

void Logger::Log(const std::string& logMessage, const LogType logType)
{
	const auto logTimeStamp = std::chrono::system_clock::now();
    const auto logTime_t = std::chrono::system_clock::to_time_t(logTimeStamp);
    const auto localTime = std::localtime(&logTime_t);

	// Can lock mutex here if we want to make it thread safe.

    std::cout << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << ' ' << LogTypeToColoredString(logType) << logMessage << '\n';
}
