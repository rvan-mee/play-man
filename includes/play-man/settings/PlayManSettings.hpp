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

/**
 * @brief Struct containing all settings for play-man not specific to any emulator.
 */
struct PlayManSettings : public ISettings<PlayManSettings>
{
	Logger::LogLevel logLevel;
	static constexpr Logger::LogLevel defaultLogLevel = Logger::LogLevel::Normal;

	std::filesystem::path logDirectory;
	static constexpr std::string_view defaultLogDirectory = "Logging";

	/**
	 * @brief -.
	 */
	PlayManSettings()
		: logLevel(defaultLogLevel)
		, logDirectory(defaultLogDirectory)
		, screenScaleGameBoy(defaultScreenScaleGameBoy)
	{}

	/**
	 * @brief -.
	 * 
	 * @param rhs 
	 */
	PlayManSettings(const PlayManSettings& rhs);

	/**
	 * @brief -.
	 * 
	 * @param rhs 
	 * @return
	 */
	PlayManSettings& operator = (const PlayManSettings& rhs)
	{
		logLevel = rhs.logLevel;
		logDirectory = rhs.logDirectory;
		screenScaleGameBoy = rhs.screenScaleGameBoy;
		return *this;
	}
	
	Logger::LogLevel logLevel;
	static constexpr Logger::LogLevel defaultLogLevel = Logger::LogLevel::Normal;

	std::filesystem::path logDirectory;
	static constexpr std::string_view defaultLogDirectory = "Logging";

	size_t screenScaleGameBoy;
	static constexpr size_t defaultScreenScaleGameBoy = 4;
};

/**
 * @brief 
 * 
 * @param j 
 * @param p 
 */
void to_json(nlohmann::json& j, const PlayManSettings& p);

/**
 * @brief 
 * 
 * @param j 
 * @param p 
 */
void from_json(const nlohmann::json& j, PlayManSettings& p);
