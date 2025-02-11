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

#include <play-man/utility/UtilFunc.hpp>

#include <ctime>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace Utility
{

	std::string CurrentTimeAsString(const std::string& format)
	{
		const auto logTimeStamp = std::chrono::system_clock::now();
		const auto logTime_t = std::chrono::system_clock::to_time_t(logTimeStamp);
		const auto localTime = std::localtime(&logTime_t);

		std::stringstream sstream;
		sstream << std::put_time(localTime, format.c_str());
		return sstream.str();
	}

	std::string ReadFileToString(const std::filesystem::path& fileToReadFrom)
	{
		std::ifstream file(fileToReadFrom);
		if (!file.good())
		{
			throw std::runtime_error("ReadFileToString failed, could not open file");
		}
		return ReadFileToString(file);
	}

	std::string ReadFileToString(std::ifstream& fileToReadFrom)
	{
		std::stringstream buffer;
		buffer << fileToReadFrom.rdbuf();
		return buffer.str();
	}

	void Replace(
		std::string& stringToReplaceIn,
		const std::string& wordToReplace,
		const std::string& wordToReplaceWith)
	{
		for (size_t pos = 0; (pos = stringToReplaceIn.find(wordToReplace, pos)) != std::string::npos; )
		{
			stringToReplaceIn.replace(pos, wordToReplace.length(), wordToReplaceWith);
			pos += wordToReplaceWith.length();
		}
	}

	std::string SanitizePathString(std::string path)
	{
	#if defined(__linux__) || defined(__APPLE__)
		constexpr auto toReplace = "\\";
		constexpr auto toReplaceWith = "/";
	#elif defined(_WIN32)
		constexpr auto toReplace = "/";
		constexpr auto toReplaceWith = "\\";
	#endif
		Replace(path, toReplace, toReplaceWith);

		return path;
	}

} /* namespace Utility */
