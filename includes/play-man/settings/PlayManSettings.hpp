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

#include "ISettings.hpp"
#include <play-man/logger/Logger.hpp>

#include <filesystem>

struct PlayManSettings : public ISettings<PlayManSettings>
{
	Logger::LogLevel logLevel;
	static constexpr Logger::LogLevel defaultLogLevel = Logger::LogLevel::Normal;
	std::filesystem::path logDirectory;
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
	p.logLevel = j.value<Logger::LogLevel>("logLevel", PlayManSettings::defaultLogLevel);
	p.logDirectory = j.value<std::filesystem::path>("logDirectory", PlayManSettings::defaultLogDirectory);
}
