
#include "play-man/logger/Logger.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

# define COLOR_RESET	 "\033[0m"
# define COLOR_ERROR	 "\033[38;5;160m"
# define COLOR_WARNING	 "\033[38;5;208m"
# define COLOR_INFO		 "\033[38;5;13m"
# define COLOR_DEBUG	 "\033[38;5;21m"

Logger& Logger::GetInstance()
{
	static Logger logger;
	return logger;
}

constexpr std::string_view Logger::LogTypeToString(LogType logType) const
{
	switch (logType)
	{
		case LogType::Error:
			return COLOR_ERROR "[ERROR]: " COLOR_RESET;
		case LogType::Warning:
			return COLOR_WARNING "[WARNING]: " COLOR_RESET;
		case LogType::Info:
			return COLOR_INFO "[INFO]: " COLOR_RESET;
		case LogType::Debug:
			return COLOR_DEBUG "[Debug]: " COLOR_RESET;
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

    std::cout << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << ' ' << LogTypeToString(logType) << logMessage << '\n';
}

#undef COLOR_RESET
#undef COLOR_ERROR
#undef COLOR_WARNING
#undef COLOR_INFO
#undef COLOR_DEBUG
