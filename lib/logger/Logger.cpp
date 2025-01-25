
#include "play-man/logger/Logger.hpp"
#include "play-man/utility/MetaUtility.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

namespace Logger
{

	LogInterface& LogInterface::GetInstance()
	{
		static LogInterface logger;
		return logger;
	}

	constexpr std::string_view LogInterface::LogTypeToColoredString(LogType logType) const
	{
		static constexpr std::string_view openingBracket = "[";
		static constexpr std::string_view closingBracket = "]: ";

		switch (logType)
		{
			case LogType::Error:
				return MetaUtility::ConcatenateStringViews_v<
					colorError, openingBracket, GetEnumAsString(logType), closingBracket, colorReset
				>;
			//case LogType::Warning:
			//	return MetaUtility::ConcatenateStringViews_v<
			//		colorWarning, openingBracket, GetEnumAsString(LogType::Warning), closingBracket, colorReset
			//	>;
			//case LogType::Info:
			//	return MetaUtility::ConcatenateStringViews_v<
			//		colorInfo, openingBracket, GetEnumAsString(LogType::Info), closingBracket, colorReset
			//	>;
			//case LogType::Debug:
			//	return MetaUtility::ConcatenateStringViews_v<
			//		colorDebug, openingBracket, GetEnumAsString(LogType::Debug), closingBracket, colorReset
			//	>;
			default:
				throw std::runtime_error("Tried to get string for unknown logtype: " + std::to_string(static_cast<int>(logType)));
		}
		return "";
	}

	void LogInterface::Log(const std::string& logMessage, const LogType logType)
	{
		const auto logTimeStamp = std::chrono::system_clock::now();
		const auto logTime_t = std::chrono::system_clock::to_time_t(logTimeStamp);
		const auto localTime = std::localtime(&logTime_t);

		// Can lock mutex here if we want to make it thread safe.

		std::cout << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << ' ' << LogTypeToColoredString(logType) << logMessage << '\n';
	}

} /* namespace logger */

