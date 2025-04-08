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

#include <play-man/utility/JsonUtility.hpp>
#include <play-man/utility/UtilFunc.hpp>
#include <play-man/logger/Logger.hpp>

#include <string>
#include <filesystem>
#include <memory>

/**
 * @brief Basic interface for a settings object.
 *        To make the class serializable to json you have to define
 *        the `to_json` and `from_json` outside of your class but within
 *        the same namespace your class is defined. This does require the class to
 *        be default constructable.
 *        For more information see https://json.nlohmann.me/features/arbitrary_types/.
*/
template <class Derived>
class ISettings
{
	Derived& derived; /* Reference to the derived instance of this object. */

protected:

	/**
	 * @brief
	 */
	ISettings() : derived(*(static_cast<Derived*>(this)))
	{
	}

	/**
	 * @brief
	 * 
	 * @param rhs 
	 * @return
	 */
	ISettings& operator = (const ISettings& rhs)
	{
		(void)rhs;
		return *this;
	}

public:

	/**
	 * @brief Instantiates Derived from json file.
	 * 
	 * @param fileToReadSettingsFrom 
	 * @return
	 */
	static std::shared_ptr<Derived> ReadFromFile(const std::filesystem::path& fileToReadSettingsFrom)
	{
		nlohmann::json settingsAsJson;
		settingsAsJson = Utility::Json::ReadJsonFromFile(fileToReadSettingsFrom);
		return std::make_shared<Derived>(settingsAsJson.template get<Derived>());
	}

	/**
	 * @brief 
	 */
	void ResetToDefaults()
	{
		derived = Derived();
	}

	/**
	 * @brief Saves settings to file as json.
	 * @param fileToSaveSettingsTo 
	 */
	void SaveToFile(const std::filesystem::path fileToSaveSettingsTo) const
	{
		const auto absolutePath = std::filesystem::absolute(fileToSaveSettingsTo);
		const auto containingDirectory = absolutePath.parent_path();
		if (!std::filesystem::exists(containingDirectory))
		{
			std::filesystem::create_directories(containingDirectory);
		}

		std::ofstream fileToSaveTo(absolutePath);
		fileToSaveTo << ToString() << std::endl;
	}

	/**
	 * @brief Serializes the class into a formatted json string.
	 * @return
	 */
	std::string ToString() const
	{
		const nlohmann::json serializedClass = ToJson();
		return serializedClass.dump(Utility::Json::numberOfSpacesIndentation);
	}

	/**
	 * @brief Serializes the class into json.
	 * @return
	 */
	nlohmann::json ToJson() const
	{
		nlohmann::json json = derived;
		return json;
	}

};
