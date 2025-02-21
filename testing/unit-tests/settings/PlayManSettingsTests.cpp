#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/settings/PlayManSettings.hpp"
#include "play-man/utility/JsonUtility.hpp"

namespace TestFixtures
{

	/**
	 * @brief
	 */
	struct PlayManSettingsFixture
	{
	private:

		std::vector<std::filesystem::path> filesToRemoveUponDestruction;
		
	public:

		std::shared_ptr<PlayManSettings> settings;

		const std::string testDataDir = "test-data/settings/";
	
		PlayManSettingsFixture() : settings(std::make_shared<PlayManSettings>())
		{
		}

		~PlayManSettingsFixture()
		{
			for (const auto& file : filesToRemoveUponDestruction)
			{
				std::filesystem::remove(file);
			}
		}

		/**
		 * @brief Files crated here will be deleted when the fixture is destroyed.
		 */
		void SaveSettingsToFile(const std::filesystem::path& file)
		{
			filesToRemoveUponDestruction.emplace_back(file);
			settings->SaveToFile(file);
		}

	};
}

TEST_CASE_METHOD(TestFixtures::PlayManSettingsFixture, "Generate Default settings")
{
	REQUIRE(settings->logDirectory == settings->defaultLogDirectory);
	REQUIRE(settings->logLevel == settings->defaultLogLevel);
}

TEST_CASE_METHOD(TestFixtures::PlayManSettingsFixture, "Load partial settings")
{
	settings = PlayManSettings::ReadFromFile(testDataDir + "partialPlayManSettings.json");
	REQUIRE(settings->logDirectory == settings->defaultLogDirectory);
	REQUIRE(settings->logLevel == Logger::LogLevel::None);
}

TEST_CASE_METHOD(TestFixtures::PlayManSettingsFixture, "Saving settings")
{
	settings->logLevel = Logger::LogLevel::Debug;
	settings->logDirectory = "unitTestLogDirectory";

	constexpr auto fileName = "savingSettings.json";

	SaveSettingsToFile(fileName);

	auto newSettings = PlayManSettings::ReadFromFile(fileName);
	
	REQUIRE(newSettings->logLevel == Logger::LogLevel::Debug);
	REQUIRE(newSettings->logDirectory == "unitTestLogDirectory");
}

TEST_CASE_METHOD(TestFixtures::PlayManSettingsFixture, "Reset to default settings")
{
	settings->logDirectory = "changed";
	settings->logLevel = Logger::LogLevel::Sparse;

	settings->ResetToDefaults();

	REQUIRE(settings->logDirectory == settings->defaultLogDirectory);
	REQUIRE(settings->logLevel == settings->defaultLogLevel);
}

TEST_CASE_METHOD(TestFixtures::PlayManSettingsFixture, "Invalid settings")
{
	REQUIRE_THROWS(settings->ReadFromFile(testDataDir + "invalidSettings.json"));
}
