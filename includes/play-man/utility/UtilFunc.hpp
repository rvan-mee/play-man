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

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace Utility
{
	/**
	 * @brief Converts any size unsigned integer into a hex string.
	 */
	template<typename T>
	std::string IntAsHexString(const T value)
	{
		std::stringstream stream;

		stream << "0x" << std::setfill ('0') << std::setw(sizeof(T) * 2) << std::hex << std::uppercase << +value;
		return (stream.str());
	}

	/**
	 * @brief 
	 * @return
	 */
	std::string CurrentTimeAsString(const std::string& format = "%Y-%m-%d %H.%M.%S");

	/**
	 * @brief
	 * 
	 * @param fileToReadFrom 
	 * @return
	 */
	std::string ReadFileToString(const std::filesystem::path& fileToReadFrom);

	/**
	 * @brief
	 * 
	 * @param fileToReadFrom 
	 * @return
	 */
	std::string ReadFileToString(std::ifstream& fileToReadFrom);

	/**
	 * @brief Note that this function replaces it in stringToReplace in.
	 *        If you still need the original value just make a copy.
	 * 
	 * @param stringToReplaceIn 
	 * @param wordToReplace 
	 * @param wordToReplaceWith 
	 */
	void Replace(
		std::string& stringToReplaceIn,
		const std::string& wordToReplace,
		const std::string& wordToReplaceWith);

	/**
	 * @brief Because windows does other slashes than linux :/
	 * 
	 * @param path 
	 * @return std::string 
	 */
	std::string SanitizePathString(std::string path);

	/**
	 * @brief Returns the associated error message of the current errno value.
	 * 
	 * @return std::string 
	 */
	std::string ErrnoToString();


} /* namespace Utility */
