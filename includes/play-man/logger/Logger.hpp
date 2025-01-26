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
#include <fstream>

namespace Logger
{

	#define LOG_TYPE_SEQ(X, n) \
		X(n, Error) \
		X(n, Warning) \
		X(n, Info) \
		X(n, Debug)

	/**
	 * @brief Enum describing the severity of the log.
	 */
	CREATE_ENUM_WITH_UTILS(LOG_TYPE_SEQ, LogType)
	#undef LOG_TYPE_SEQ

	/*!
	* @brief Singleton logger for a unified and not yet threadsafe output.
	* 		 The logger currently writes to std::cout and to a log file. 
	*		 Each time the logger is instaniated it will create a new file with
	*		 the timestamp of when it was created, making it easy to find
	*		 the logs you are looking for.
	*/
	class LogInterface
	{
		/*
		 * Defined as static constexpr because otherwise the concatenation wouldnt work 
		*/
		static constexpr std::string_view errorHeader	= "[Error]: ";
		static constexpr std::string_view warningHeader	= "[Warning]: ";
		static constexpr std::string_view infoHeader	= "[Info]: ";
		static constexpr std::string_view debugHeader	= "[Debug]: ";

		static constexpr std::string_view colorReset	= "\033[0m";
		static constexpr std::string_view colorError	= "\033[38;5;160m";
		static constexpr std::string_view colorWarning	= "\033[38;5;208m";
		static constexpr std::string_view colorInfo		= "\033[38;5;13m";
		static constexpr std::string_view colorDebug	= "\033[38;5;21m";
		static constexpr std::string_view colorGray		= "\033[38;5;232m";

		std::ofstream logFile; /*!< -. */
		bool coutLoggingEnabled; /*!< -. */

		/*!
		* @brief Hidden default constructor.
		*/
		LogInterface();

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
		LogInterface& operator = (const LogInterface&) = delete;

		/**
		 * @brief 
		 * 
		 * @param logType 
		 * @return constexpr std::string_view 
		 */
		static constexpr std::string_view LogTypeHeader(LogType logType);

		/**
		 * @brief -.
		 * 
		 * @param logType 
		 * @return constexpr std::string_view 
		 */
		static constexpr std::string_view LogTypeHeaderColored(LogType logType);

	public:

		const std::string logDir = "Logging";

		/**
		 * @brief
		 * 
		 * @return 
		 */
		static LogInterface& GetInstance();

		/**
		 * @brief -.
		 * 
		 * @param enabled 
		 */
		void EnableCoutLogging(bool enabled);

		/*!
		* @brief Not yet thread safe function for writing a log to stdout.
		*		  Adds a timestamp and the logtype as prefix to the logmessage.
		* @param logMessage
		* @param logType
		*/
		void Log(const std::string& logMessage, const LogType logType);

	};

} /* namespace Logger */


#define LOG_ERROR(logMessage) Logger::LogInterface::GetInstance().Log(logMessage, Logger::LogType::Error);
#define LOG_WARNING(logMessage) Logger::LogInterface::GetInstance().Log(logMessage, Logger::LogType::Warning);
#define LOG_INFO(logMessage) Logger::LogInterface::GetInstance().Log(logMessage, Logger::LogType::Info);
#define LOG_DEBUG(logMessage) Logger::LogInterface::GetInstance().Log(logMessage, Logger::LogType::Debug);
