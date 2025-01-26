
#include "play-man/logger/Logger.hpp"
#include "play-man/utility/MetaUtility.hpp"
#include "play-man/utility/UtilFunc.hpp"

#include <iostream>
#include <filesystem>

namespace Logger
{

	LogInterface::LogInterface() : coutLoggingEnabled(true)
	{
		std::filesystem::create_directory(logDir);
		logFile = std::ofstream(logDir + "/play-man-" + Utility::CurrentTimeAsString());
	}

	LogInterface& LogInterface::GetInstance()
	{
		static LogInterface logger;
		return logger;
	}

	constexpr std::string_view LogInterface::LogTypeHeader(LogType logType)
	{
		switch (logType)
		{
			case LogType::Error:
				return errorHeader;
			case LogType::Warning:
				return warningHeader;
			case LogType::Info:
				return infoHeader;
			case LogType::Debug:
				return debugHeader;
			default:
				throw std::runtime_error("Tried to get header for unknown logtype: " + std::to_string(static_cast<int>(logType)));
		}
		return "";
	}

	constexpr std::string_view LogInterface::LogTypeHeaderColored(LogType logType)
	{
		switch (logType)
		{
			case LogType::Error:
				return MetaUtility::ConcatenateStringViews<
					colorError, errorHeader, colorReset
				>::value;
			case LogType::Warning:
				return MetaUtility::ConcatenateStringViews<
					colorWarning, warningHeader, colorReset
				>::value;
			case LogType::Info:
				return MetaUtility::ConcatenateStringViews<
					colorInfo, infoHeader, colorReset
				>::value;
			case LogType::Debug:
				return MetaUtility::ConcatenateStringViews<
					colorDebug, debugHeader, colorReset
				>::value;
			default:
				throw std::runtime_error("Tried to get string for unknown logtype: " + std::to_string(static_cast<int>(logType)));
		}
		return "";
	}

	void LogInterface::Log(const std::string& logMessage, const LogType logType)
	{
		logFile << Utility::CurrentTimeAsString() << ' ' << LogTypeHeader(logType) << logMessage << '\n';

		if (coutLoggingEnabled)
		{
			std::cout << colorGray << Utility::CurrentTimeAsString() << ' ' << LogTypeHeaderColored(logType) << logMessage << '\n';
		}
	}

	void LogInterface::EnableCoutLogging(bool enabled)
	{
		coutLoggingEnabled = enabled;
	}

} /* namespace logger */

