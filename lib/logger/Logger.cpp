
#include "play-man/logger/Logger.hpp"
#include "play-man/utility/MetaUtility.hpp"
#include "play-man/utility/UtilFunc.hpp"

#ifndef PLAY_MAN_NO_COUT_LOGGING
	# include <iostream>
#endif

#ifndef PLAY_MAN_NO_FILE_LOGGING
	# include <filesystem>
#endif

namespace Logger
{

	LogInterface::LogInterface(const std::string& logDir_, LogLevel logLevel_)
		: logDir(logDir_)
		, logFileName(logDir + "/play-man-" + Utility::CurrentTimeAsString() + ".log")
		, logLevel(logLevel_)
	{
	#ifndef PLAY_MAN_NO_FILE_LOGGING
		std::filesystem::create_directory(logDir);
		logFile.open(logFileName);

		if (!logFile.good())
		{
			throw std::runtime_error("Unable to open logFile in " + logDir);
		}
	#endif
	}

	std::unique_ptr<LogInterface>& LogInterface::GetInstance()
	{
		static std::unique_ptr<LogInterface> logger;
		return logger;
	}

	void LogInterface::Initialize(const std::string& logDir, LogLevel logLevel)
	{
		auto& logger = GetInstance();

		if (logger != nullptr)
		{
			throw std::runtime_error("Logger is already initialized.");
		}
	
		logger = std::unique_ptr<LogInterface>(new LogInterface(logDir, logLevel));
	}

	std::string_view LogInterface::LogTypeHeader(LogType logType)
	{
		switch (logType)
		{
			case LogType::Fatal:
				return fatalHeader;
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
	}

	std::string_view LogInterface::LogTypeHeaderColored(LogType logType)
	{
		switch (logType)
		{
			case LogType::Fatal:
				return MetaUtility::ConcatenateStringViews<
					colorFatal, fatalHeader, colorReset
				>::value;
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

	bool LogInterface::CanLogLogWithSetLogLevel(LogType logType)
	{
		switch (logLevel)
		{
			case LogLevel::None:	return false;
			case LogLevel::Sparse:	return (logType != LogType::Debug && logType != LogType::Info);
			case LogLevel::Normal:	return (logType != LogType::Debug);
			case LogLevel::Debug:	return true;
			default:
				return false;
		}
	}

	LogInterface& LogInterface::WriteLogHeader(LogType logType)
	{
		// Setting lastLoggedType before checking because if it was written for the
		// streaming operator it needs to be set so the later streams will not be written
		// because of a previous log message.
		lastLoggedType = logType;
		if (!CanLogLogWithSetLogLevel(logType))
		{
			return *this;
		}

		const auto logTimeString = Utility::CurrentTimeAsString();

	#ifndef PLAY_MAN_NO_FILE_LOGGING
		logFile << logTimeString << ' ' << LogTypeHeader(logType);
	#endif

	#ifndef PLAY_MAN_NO_COUT
		std::cout << colorGray << logTimeString << ' ' << LogTypeHeaderColored(logType);
	#endif

		return *this;
	}

	void LogInterface::LogWithoutHeader(const std::string& logMessage)
	{
	#ifndef PLAY_MAN_NO_FILE_LOGGING
		logFile << logMessage;
	#endif

	#ifndef PLAY_MAN_NO_COUT
		std::cout << logMessage;
	#endif
	}

	void LogInterface::Log(const std::string& logMessage, const LogType logType)
	{
		if (!CanLogLogWithSetLogLevel(logType))
		{
			return;
		}

		(void)WriteLogHeader(logType);
		LogWithoutHeader(logMessage);
		LogWithoutHeader("\n");
	}

} /* namespace logger */

