#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/logger/Logger.hpp"
#include "play-man/utility/UtilFunc.hpp"
#include <regex>

namespace TestFixtures
{

	/**
	 * @brief Fixture for testing the logger which resets the logger upon destruction.
	 */
	struct LoggerTestFixture
	{
		const std::string unitTestLogDir = "unit-test-logging";
		const std::string testLogMessage = "this is a test log message";

		std::unique_ptr<Logger::LogInterface>& logger = Logger::LogInterface::GetInstance();

		~LoggerTestFixture()
		{
			Logger::LogInterface::GetInstance().reset();
		}

		std::string RegexLogTypeHeader(Logger::LogType logType)
		{
			return "\\[" + std::string(GetEnumAsString(logType)) + "\\]: ";
		}

		bool TestLogMessageIsLogged(Logger::LogType logType)
		{
			
			const std::string regexDateTimeString = "\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d.\\d\\d.\\d\\d ";
			const std::regex regexToMatch(regexDateTimeString + RegexLogTypeHeader(logType) + testLogMessage);

			// Flushing the buffer because I was going bald trying to figure out why the file was empty :/
			logger->logFile.flush();
			std::ifstream f(logger->logFileName);
			for (std::string line; std::getline(f, line) ; )
			{
				if (std::regex_match(line, regexToMatch))
				{
					return true;
				}
			}
			return false;
		}

	};

} /* namespace TestFixtures*/



TEST_CASE_METHOD(TestFixtures::LoggerTestFixture, "File logging")
{
	bool shouldInfoBeLogged = true;
	bool shouldWarningBeLogged = true;
	bool shouldErrorBeLogged = true;
	bool shouldFatalBeLogged = true;
	bool shouldDebugBeLogged = true;

	Logger::LogLevel logLevelToTest;

	SECTION("LogLevel Debug")
	{
		logLevelToTest = Logger::LogLevel::Debug;
	}
	SECTION("LogLevel Normal")
	{
		logLevelToTest = Logger::LogLevel::Normal;
		shouldDebugBeLogged = false;
	}
	SECTION("LogLevel Sparse")
	{
		logLevelToTest = Logger::LogLevel::Sparse;
		shouldInfoBeLogged = false;
		shouldDebugBeLogged = false;
	}
	SECTION("LogLevel None")
	{
		logLevelToTest = Logger::LogLevel::None;
		shouldInfoBeLogged = false;
		shouldWarningBeLogged = false;
		shouldErrorBeLogged = false;
		shouldFatalBeLogged = false;
		shouldDebugBeLogged = false;
	}

	Logger::LogInterface::Initialize(unitTestLogDir, logLevelToTest);

	LOG_INFO(testLogMessage);
	LOG_WARNING(testLogMessage);
	LOG_ERROR(testLogMessage);
	LOG_FATAL(testLogMessage);
	LOG_DEBUG(testLogMessage);

	INFO("Testing loglevel " + std::string(GetEnumAsString(logLevelToTest)));

	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Info) == shouldInfoBeLogged);
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Warning) == shouldWarningBeLogged);
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Error) == shouldErrorBeLogged);
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Fatal) == shouldFatalBeLogged);
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Debug) == shouldDebugBeLogged);
}
