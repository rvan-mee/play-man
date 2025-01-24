#pragma once

#include "ErrorCodes.hpp"

#include <string>

/**
 * @brief
 */
enum class IssueType
{
	Warning,
	Error,
	Debug
};

/**
 * @brief
 */
enum class IssueResolvable
{
	ManuallyResolvable, // Click on Ok or whatever to resolve.
	AutomaticResolvable, // Can be automatically resolved in the code.
	ResetResolvable // Program needs to be restarted.
};

/**
 * @brief
 */
struct Issue
{
	const ErrorCode errorCode; /* */
	const IssueType issueType; /* */
	const IssueResolvable issueResolvable; /* */
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
