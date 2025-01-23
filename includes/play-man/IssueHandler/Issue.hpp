#pragma once

#include "ErrorCodes.hpp"

#include <string>

/**\
 * @brief
 */
struct Issue
{
	const ErrorCode errorCode; /* */
	const std::string moduleName; /* */
	const std::string errorMessage; /* */
	const std::string detailedErrorMessage; /* */
	const bool showAsPopup; /* */

	/**
	 * @brief -.
	 * 
	 * @param lhs 
	 * @param rhs 
	 * @return
	 */
	friend bool operator == (const Issue& lhs, const Issue& rhs);

	/**
	 * @brief -.
	 * 
	 * @param lhs 
	 * @param rhs 
	 * @return
	 */
	friend bool operator != (const Issue& lhs, const Issue& rhs);

	/**
	 * @brief Not a very logical operator, but here just so they can be stored in a map or set.
	 * 
	 * @param lhs 
	 * @param rhs 
	 * @return
	 */
	friend bool operator < (const Issue& lhs, const Issue& rhs);
};
