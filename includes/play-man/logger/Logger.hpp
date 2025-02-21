// ****************************************************************************** //
//   _______   __                              __       __                        //
//  /       \ /  |                            /  \     /  |                       //
//  $$$$$$$  |$$ |  ______   __    __         $$  \   /$$ |  ______   _______     //
//  $$ |__$$ |$$ | /      \ /  |  /  | ______ $$$  \ /$$$ | /      \ /       \    //
//  $$    $$/ $$ | $$$$$$  |$$ |  $$ |/      |$$$$  /$$$$ | $$$$$$  |$$$$$$$  |   //
//  $$$$$$$/  $$ | /    $$ |$$ |  $$ |$$$$$$/ $$ $$ $$/$$ | /    $$ |$$ |  $$ |   //
//  $$ |      $$ |/$$$$$$$ |$$ \__$$ |        $$ |$$$/ $$ |/$$$$$$$ |$$ |  $$ |   //
//  $$ |      $$ |$$    $$ |$$    $$ |        $$ | $/  $$ |$$    $$ |$$ |  $$ |   //
//  $$/       $$/  $$$$$$$/  $$$$$$$ |        $$/      $$/  $$$$$$$/ $$/   $$/    //
//                          /  \__$$ |                                            //
//                          $$    $$/                                             //
//                           $$$$$$/                                              //
//                                                                                //
//                            By: K1ngmar and rvan-mee                            //
// ****************************************************************************** //

#pragma once

#include <play-man/utility/EnumMacro.hpp>

#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <filesystem>

#ifndef PLAY_MAN_NO_FILE_LOGGING
	#include <fstream>
#endif

namespace TestFixtures
{
	// Forward declaration for unit testing purposes.
	struct LoggerTestFixture;
}

namespace Logger
{

	#define LOG_TYPE_SEQ(X, n) \
		X(n, Fatal) \
		X(n, Error) \
		X(n, Warning) \
		X(n, Info) \
		X(n, Debug)

	/**
	 * @brief Enum describing the severity of the log.
	 */
	CREATE_ENUM_WITH_UTILS(LOG_TYPE_SEQ, LogType)
	#undef LOG_TYPE_SEQ

	#define LOG_EVEL_SEQ(X, n) \
		X(n, None) \
		X(n, Sparse) \
		X(n, Normal) \
		X(n, Debug)

	/**
	 * @brief Enum describing how much to log.
	 *        Debug: Logs all log types.
	 *        Normal: Logs everything except debug.
	 *        Sparse: Only logs Error, Fatal and warnings.
	 *        None: self explanatory.
	 */
	CREATE_ENUM_WITH_UTILS(LOG_EVEL_SEQ, LogLevel)
	#undef LOG_EVEL_SEQ

	/*!
	* @brief Singleton logger for a unified and not yet threadsafe output.
	* 		 The logger currently writes to std::cout and to a log file. 
	*		 Each time the logger is instaniated it will create a new file with
	*		 the timestamp of when it was created, making it easy to find
	*		 the logs you are looking for.
	*/
	class LogInterface
	{
		// For testing purposes.
		friend struct TestFixtures::LoggerTestFixture;

		/*
		 * Defined as static constexpr because otherwise the concatenation wouldnt work.
		*/
		static constexpr std::string_view fatalHeader	= "[Fatal]: ";
		static constexpr std::string_view errorHeader	= "[Error]: ";
		static constexpr std::string_view warningHeader	= "[Warning]: ";
		static constexpr std::string_view infoHeader	= "[Info]: ";
		static constexpr std::string_view debugHeader	= "[Debug]: ";

		static constexpr std::string_view colorReset	= "\033[0m";
		static constexpr std::string_view colorFatal	= "\033[38;5;226m";
		static constexpr std::string_view colorError	= "\033[38;5;160m";
		static constexpr std::string_view colorWarning	= "\033[38;5;208m";
		static constexpr std::string_view colorInfo		= "\033[38;5;13m";
		static constexpr std::string_view colorDebug	= "\033[38;5;21m";
		static constexpr std::string_view colorGray		= "\033[38;5;232m";


	#ifndef PLAY_MAN_NO_FILE_LOGGING
		const std::filesystem::path logDir;
		const std::string logFileName;
		std::ofstream logFile;
	#endif

		LogLevel logLevel;
		std::optional<LogType> lastLoggedType; // LogType of the previous message logged, mainly usefull for the streaming operator.

		/*!
		* @brief Hidden default constructor.
		*/
		LogInterface(const std::filesystem::path& logDir, LogLevel logLevel);

		/*!
		* @brief
		* @param
		*/
		LogInterface(const LogInterface&) = delete;

		/*!
		* @brief
		* @param
		* @return
		*/
		std::unique_ptr<LogInterface>& operator = (const LogInterface&) = delete;

		/**
		 * @brief 
		 * 
		 * @param logType 
		 * @return
		 */
		static std::string_view LogTypeHeader(LogType logType);

		/**
		 * @brief -.
		 * 
		 * @param logType 
		 * @return
		 */
		static std::string_view LogTypeHeaderColored(LogType logType);

		/**
		 * @brief
		 * 
		 * @param logMessage 
		 */
		void LogWithoutHeader(const std::string& logMessage);

		/**
		 * @brief 
		 * 
		 * @param logType 
		 * @return true 
		 * @return false 
		 */
		bool CanLogLogWithSetLogLevel(LogType logType);

	public:

		/*!
		* @brief
		* @return The singleton instance.
		*/
		static std::unique_ptr<LogInterface>& GetInstance();

		/**
		 * @brief Initializes the instance, logging without initializing is not possible.
		 *        It is still a signleton because if we ever want to mulithread this guarrantees one instance.
		 * 
		 * @param logDir
		 * @param logLevel
		 */
		static void Initialize(const std::filesystem::path& logDir, LogLevel logLevel);

		/*!
		* @brief Not yet thread safe function for writing a log to stdout.
		*		  Adds a timestamp and the logtype as prefix to the logmessage.
		* @param logMessage
		* @param logType
		*/
		void Log(const std::string& logMessage, const LogType logType);

		/**
		 * @brief Writes the right logHeader and returns a reference to the interface
		 * 		  making it possible to stream to the logger after writing the header.
		 *  
		 * @param logType
		 */
		LogInterface& WriteLogHeader(LogType logType);
		
		template <class T>
		friend LogInterface& operator << (LogInterface& logInterface, const T& toWrite);

	};

	/**
	 * @brief Note that when stream operator for writing logs you will have to write
	 * 		  the logHeader and the newline yourself.
	 *        For writing the header you can use ie `LOG_ERROR_STREAM << "message"`;
	 *        Do note that this is slower than logging through the LOG() function.
	 * 
	 * @tparam T 
	 * @param logInterface 
	 * @param toWrite 
	 * @return
	 */
	template <class T>
	LogInterface& operator << (LogInterface& logInterface, const T& toWrite)
	{
		if (!logInterface.lastLoggedType.has_value() ||
			!logInterface.CanLogLogWithSetLogLevel(logInterface.lastLoggedType.value())
			)
		{
			return logInterface;
		}

		logInterface.LogWithoutHeader(toWrite);
		return logInterface;
	}

} /* namespace Logger */

#define LOG_ERROR_STREAM Logger::LogInterface::GetInstance()->WriteLogHeader(Logger::LogType::Error)
#define LOG_FATAL_STREAM Logger::LogInterface::GetInstance()->WriteLogHeader(Logger::LogType::Fatal)
#define LOG_WARNING_STREAM Logger::LogInterface::GetInstance()->WriteLogHeader(Logger::LogType::Warning)
#define LOG_INFO_STREAM Logger::LogInterface::GetInstance()->WriteLogHeader(Logger::LogType::Info)
#define LOG_DEBUG_STREAM Logger::LogInterface::GetInstance()->WriteLogHeader(Logger::LogType::Debug)

#define LOG_ERROR(logMessage) Logger::LogInterface::GetInstance()->Log(logMessage, Logger::LogType::Error);
#define LOG_FATAL(logMessage) Logger::LogInterface::GetInstance()->Log(logMessage, Logger::LogType::Fatal);
#define LOG_WARNING(logMessage) Logger::LogInterface::GetInstance()->Log(logMessage, Logger::LogType::Warning);
#define LOG_INFO(logMessage) Logger::LogInterface::GetInstance()->Log(logMessage, Logger::LogType::Info);
#define LOG_DEBUG(logMessage) Logger::LogInterface::GetInstance()->Log(logMessage, Logger::LogType::Debug);
