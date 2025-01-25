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

#include <string_view>
#include <array>

namespace MetaUtility
{
	/**
	 * @brief 
	 * 
	 * @tparam T 
	 * @tparam Views 
	 */
	template <std::string_view const&... Views>
	class ConcatenateStringViews
	{
		/**
		 * @brief Concatenates all views and returns them as an array.
		 * 
		 * @return constexpr auto 
		 */
		static constexpr auto Impl() noexcept
		{
			constexpr size_t totalLength = (Views.size() + ... + 0);

			std::array<char, totalLength + 1> concatenatedArray_{};
			size_t i = 0;

			// Use a lambda for appending so we can use a fold expresion to append all of them.
			auto append = [&](auto const& s) mutable
			{
				for (auto c : s)
				{
					concatenatedArray_[i++] = c;
				}
			};
			(append(Views), ...);
			concatenatedArray_[totalLength] = 0;
			return concatenatedArray_;
		}
		
		static constexpr auto concatenatedArray = Impl();

	public:

		static constexpr std::string_view value {concatenatedArray.data(), concatenatedArray.size() - 1};
	};

	/**
	 * @brief Concatenates all views and returns them as an array.
	 * 
	 * @return constexpr auto 
	 */
	template <std::string_view const&... Views>
	using ConcatenateStringViews_v = typename ConcatenateStringViews<Views...>::value;

} /* namespace MetaUtility */
