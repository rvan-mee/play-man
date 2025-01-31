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
			
			const std::string regexDateTimeString = "\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d:\\d\\d:\\d\\d ";
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



TEST_CASE_METHOD(TestFixtures::LoggerTestFixture, "LogLevel debug")
{
	Logger::LogInterface::Initialize(unitTestLogDir, Logger::LogLevel::Debug);

	LOG_INFO(testLogMessage);
	LOG_WARNING(testLogMessage);
	LOG_ERROR(testLogMessage);
	LOG_FATAL(testLogMessage);
	LOG_DEBUG(testLogMessage);

	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Info));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Warning));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Error));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Fatal));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Debug));
}

TEST_CASE_METHOD(TestFixtures::LoggerTestFixture, "LogLevel Normal")
{
	Logger::LogInterface::Initialize(unitTestLogDir, Logger::LogLevel::Normal);

	LOG_INFO(testLogMessage);
	LOG_WARNING(testLogMessage);
	LOG_ERROR(testLogMessage);
	LOG_FATAL(testLogMessage);
	LOG_DEBUG(testLogMessage);

	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Info));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Warning));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Error));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Fatal));
	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Debug));
}

TEST_CASE_METHOD(TestFixtures::LoggerTestFixture, "LogLevel None")
{
	Logger::LogInterface::Initialize(unitTestLogDir, Logger::LogLevel::Sparse);

	LOG_INFO(testLogMessage);
	LOG_WARNING(testLogMessage);
	LOG_ERROR(testLogMessage);
	LOG_FATAL(testLogMessage);
	LOG_DEBUG(testLogMessage);

	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Info));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Warning));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Error));
	REQUIRE(TestLogMessageIsLogged(Logger::LogType::Fatal));
	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Debug));
}

TEST_CASE_METHOD(TestFixtures::LoggerTestFixture, "LogLevel Debug")
{
	Logger::LogInterface::Initialize(unitTestLogDir, Logger::LogLevel::None);

	LOG_INFO(testLogMessage);
	LOG_WARNING(testLogMessage);
	LOG_ERROR(testLogMessage);
	LOG_FATAL(testLogMessage);
	LOG_DEBUG(testLogMessage);

	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Info));
	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Warning));
	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Error));
	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Fatal));
	REQUIRE(!TestLogMessageIsLogged(Logger::LogType::Debug));
}
