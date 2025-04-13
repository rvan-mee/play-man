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

#include <nlohmann/json.hpp>

#include <optional>

namespace Utility { namespace Json {

	constexpr size_t numberOfSpacesIndentation = 4; /* The number of spaces to use when indenting for serialization. */

	/**
	 * @brief Due to bug `https://github.com/nlohmann/json/issues/2046` the default
	 *        constructor of the json creates invalid/unusable json on certain compilers.
	 *        Just use this function and all should be well :).
	 * 
	 * @return
	 */
	nlohmann::json CreateEmptyJson();

	/**
	 * @brief Reads file as json.
	 * @param fileToReadFrom
	 * @return
	 */
	nlohmann::json ReadJsonFromFile(const std::filesystem::path& fileToReadFrom);

	/**
	 * @brief Get the value from the tree as T if it exists.
	 * 
	 * @tparam T 
	 * @param jsonTree 
	 * @param pathToValue 
	 * @return
	 */
	template <class T>
	std::optional<T> GetOptional(
		const nlohmann::json& jsonTree,
		const nlohmann::json::json_pointer& pathToValue);

	/**
	 * @brief Prints class of type T as formatted json.
	 * @tparam T 
	 * @param toPrint 
	 * @return
	 */
	template <class T>
	std::ostream& PrintFormatted(std::ostream& os, const T& toPrint)
	{
		nlohmann::json j;
		to_json(j, toPrint);
		os << j.dump(4);
		return os;
	}

	#include "JsonUtility.ipp"

} /* namespace Json */

} /* namespace Utility */
