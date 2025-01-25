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
	* @brief Signleton logger for a unified and threadsafe output.
	*/
	class LogInterface
	{
	public:



	private:

		static constexpr std::string_view colorReset	=	"\033[0m"; /*!< -. */
		static constexpr std::string_view colorError	=	"\033[38;5;160m"; /*!< -. */
		static constexpr std::string_view colorWarning	=	"\033[38;5;208m"; /*!< -. */
		static constexpr std::string_view colorInfo		=	"\033[38;5;13m"; /*!< -. */
		static constexpr std::string_view colorDebug	=	"\033[38;5;21m"; /*!< -. */

		constexpr std::string_view LogTypeToColoredString(LogType logType) const;

		/*!
		* @brief Hidden default constructor.
		*/
		LogInterface() = default;

		/*!
		* @brief Hidden copy constructor.
		* @param
		*/
		LogInterface(const LogInterface&) = default;

		/*!
		* @brief Hidden assignation operator.
		* @param
		* @return
		*/
		LogInterface& operator = (const LogInterface&) = default;

	public:


		/*!
		* @brief
		* @return The singleton instance.
		*/
		static LogInterface& GetInstance();

		/*!
		* @brief Thread safe function for writing a log to stdout.
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
