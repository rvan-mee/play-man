#pragma once

#include <play-man/utility/JsonUtility.hpp>
#include <play-man/utility/UtilFunc.hpp>

#include <string>
#include <filesystem>

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

	ISettings() : derived(*(static_cast<Derived*>(this)))
	{
	}

public:


	/**
	 * @brief Instantiates Derived from json file.
	 * 
	 * @param fileToReadSettingsFrom 
	 * @return
	 */
	static Derived ReadFromFile(const std::filesystem::path& fileToReadSettingsFrom)
	{
		nlohmann::json settingsAsJson;
		if (std::filesystem::exists(fileToReadSettingsFrom))
		{
			const auto settingsAsString = Utility::ReadFileToString(fileToReadSettingsFrom);
			settingsAsJson = nlohmann::json::parse(settingsAsString);
		}
		return settingsAsJson.template get<Derived>();
	}

	/**
	 * @brief 
	 */
	void ResetToDefaults()
	{
		// This is kind of abusing the from_json function as we try to convert an empty
		// json tree to an instance of the settings, if a setting is not supplied the default will be used :).
		nlohmann::json j;
		derived = j.template get<Derived>();
	}

	/**
	 * @brief Saves settings to file as json.
	 * @param fileToSaveSettingsTo 
	 */
	void SaveToFile(const std::filesystem::path fileToSaveSettingsTo) const
	{
		const auto containingDirectory = fileToSaveSettingsTo.parent_path();
		if (!std::filesystem::exists(containingDirectory))
		{
			std::filesystem::create_directories(containingDirectory);
		}

		std::ofstream fileToSaveTo(fileToSaveSettingsTo);
		fileToSaveTo << ToString() << std::endl;
	}

	/**
	 * @brief Serializes the class into a json string.
	 * @return
	 */
	std::string ToString() const
	{
		const nlohmann::json serializedClass = ToJson();
		return serializedClass.dump(Utility::Json::numberOfSpacesIndentation);
	}

	nlohmann::json ToJson() const
	{
		nlohmann::json json = derived;
		return json;
	}

};
