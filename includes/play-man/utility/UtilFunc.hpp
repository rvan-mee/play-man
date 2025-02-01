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
#include <filesystem>

namespace Utility
{
	/**
	 * @brief 
	 * @return
	 */
	std::string CurrentTimeAsString(const std::string& format = "%Y-%m-%d %H:%M:%S");

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


} /* namespace Utility */
