
#pragma once

#include <string>
#include <string_view>

/*!
 * @brief Signleton logger for a unified and threadsafe output.
*/
class Logger
{
public:

	/*!
	 * @brief
	*/
	enum class LogType
	{
		Error,
		Warning,
		Info,
		Debug
	};

private:

# define COLOR_WARNING	 
# define COLOR_INFO		 
# define COLOR_DEBUG	 

	static constexpr std::string_view colorReset =		"\033[0m"; /*!< -. */
	static constexpr std::string_view colorError =		"\033[38;5;160m"; /*!< -. */
	static constexpr std::string_view colorWarning =	"\033[38;5;208m"; /*!< -. */
	static constexpr std::string_view colorInfo =		"\033[38;5;13m"; /*!< -. */
	static constexpr std::string_view colorDebug =		"\033[38;5;21m"; /*!< -. */

	constexpr std::string_view LogTypeToColoredString(LogType logType) const;

	/*!
	 * @brief Hidden default constructor.
	*/
	Logger() = default;

	/*!
	 * @brief Hidden copy constructor.
	 * @param
	*/
	Logger(const Logger&) = default;

	/*!
	 * @brief Hidden assignation operator.
	 * @param
	 * @return
	*/
	Logger& operator = (const Logger&) = default;

public:


	/*!
	 * @brief
	 * @return The singleton instance.
	*/
	static Logger& GetInstance();

	/*!
	 * @brief Thread safe function for writing a log to stdout.
	 *		  Adds a timestamp and the logtype as prefix to the logmessage.
	 * @param logMessage
	 * @param logType
	*/
	void Log(const std::string& logMessage, const LogType logType);

};


#define LOG_ERROR(logMessage) Logger::GetInstance().Log(logMessage, Logger::LogType::Error);
#define LOG_WARNING(logMessage) Logger::GetInstance().Log(logMessage, Logger::LogType::Warning);
#define LOG_INFO(logMessage) Logger::GetInstance().Log(logMessage, Logger::LogType::Info);
#define LOG_DEBUG(logMessage) Logger::GetInstance().Log(logMessage, Logger::LogType::Debug);
