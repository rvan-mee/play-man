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

#include <play-man/settings/PlayManSettings.hpp>

PlayManSettings::PlayManSettings()
	: logLevel(defaultLogLevel)
	, logDirectory(defaultLogDirectory)
{}

PlayManSettings::PlayManSettings(const PlayManSettings& rhs)
{
	*this = rhs;
}

PlayManSettings& PlayManSettings::operator = (const PlayManSettings& rhs)
{
	logLevel = rhs.logLevel;
	logDirectory = rhs.logDirectory;
	return *this;
}

void to_json(nlohmann::json& j, const PlayManSettings& p)
{
	j = nlohmann::json {
		{"logLevel", p.logLevel},
		{"logDirectory", p.logDirectory},
		{"screenScaleGameBoy", p.screenScaleGameBoy},
	};
}

void from_json(const nlohmann::json& j, PlayManSettings& p)
{
	p.logLevel = j.value<Logger::LogLevel>("logLevel", PlayManSettings::defaultLogLevel);
	p.logDirectory = j.value<std::filesystem::path>("logDirectory", PlayManSettings::defaultLogDirectory);
	p.screenScaleGameBoy = j.value<size_t>("screenScaleGameBoy", PlayManSettings::defaultScreenScaleGameBoy);
}
