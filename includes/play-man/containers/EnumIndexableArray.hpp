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

#include <play-man/utility/Concepts.hpp>

#include <array>

/**
 * @brief Wrapper class of std::array with indexing support for enum classes of type EnumKeyType.
 * 
 * @tparam EnumKeyType 
 * @tparam T 
 * @tparam N 
 */
template<
	Utility::Concept::IsScopedEnum EnumKeyType,
	class T,
	std::size_t N
> struct EnumIndexableArray : public std::array<T, N>
{
private:

	using array = std::array<T, N>;
	using reference = typename array::reference;
	using const_reference = typename array::const_reference;

public:

	reference at(EnumKeyType pos)
	{
		return array::at(static_cast<size_t>(pos));
	}

	const_reference at(EnumKeyType pos) const
	{
		return array::at(static_cast<size_t>(pos));
	}

	reference at(size_t pos)
	{
		return array::at(pos);
	}

	const_reference at(size_t pos) const
	{
		return array::at(pos);
	}

	reference operator[](EnumKeyType pos)
	{
		return array::operator[](static_cast<size_t>(pos));
	}

	const_reference operator[](EnumKeyType pos) const
	{
		return array::operator[](static_cast<size_t>(pos));
	}

	reference operator[](size_t pos)
	{
		return array::operator[](pos);
	}

	const_reference operator[](size_t pos) const
	{
		return array::operator[](pos);
	}
};
