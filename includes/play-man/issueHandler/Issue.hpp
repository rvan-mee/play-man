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

#include "ErrorCodes.hpp"

#include <string>


#define ISSUE_TYPE_SEQ(X, n) \
	X(n, Warning) \
	X(n, Error) \
	X(n, Debug)
/**
 * @brief
 */
CREATE_ENUM_WITH_UTILS(ISSUE_TYPE_SEQ, IssueType)
#undef ISSUE_TYPE_SEQ

#define ISSUE_RESOLVABLE_SEQ(X, n) \
	X(n, ManuallyResolvable) \
	X(n, AutomaticallyResolvable) \
	X(n, Fatal)

/**
 * @brief ManuallyResolvable: needs user confirmation to resolve.
 * 		  AutomaticResolvable: the program itself can resolve it, i.e. when there
 * 							   is a lost connection and it is reconneted again.
 * 		  Fatal: The program needs to be restarted.
 */
CREATE_ENUM_WITH_UTILS(ISSUE_RESOLVABLE_SEQ, IssueResolvable)
#undef ISSUE_RESOLVABLE_SEQ

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
