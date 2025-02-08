#pragma once

#include "ISettings.hpp"
#include <play-man/logger/Logger.hpp>

struct PlayManSettings : public ISettings<PlayManSettings>
{
	Logger::LogLevel logLevel;
	static constexpr Logger::LogLevel defaultLogLevel = Logger::LogLevel::Normal;
	std::string logDirectory;
	static constexpr std::string_view defaultLogDirectory = "Logging";
};

void to_json(nlohmann::json& j, const PlayManSettings& p)
{
	j = nlohmann::json {
		{"logLevel", p.logLevel},
		{"logDirectory", p.logDirectory}
	};
}

void from_json(const nlohmann::json& j, PlayManSettings& p)
{
	p.logLevel = j.value("logLevel", PlayManSettings::defaultLogLevel);
	p.logDirectory = j.value("logDirectory", PlayManSettings::defaultLogDirectory);
}
