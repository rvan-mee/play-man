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
#include "Issue.hpp"
#include <play-man/signal/Signal.hpp>

#include <string>
#include <optional>
#include <map>
#include <memory>
#include <functional>

/**
 * @brief The intention of the issue-handler is to be able to set issues, and have callbacks on certain issues being set.
 *        This way it is easy to see what problems are active in the program/component and handle accordingly.
 */
class IssueHandler
{
	using IssueResolvedCallbackType = Signal<void()>;

	const std::string moduleName; /*! */
	const bool allowDebug; /*!< -. */
	std::map<ErrorCode, IssueResolvedCallbackType> issueResolvedCallbacks; /*!  */
	std::map<ErrorCode, IssueResolvedCallbackType> issueSetCallbacks; /*!  */
	std::map<ErrorCode, Issue> activeErrors; /*!<  */

public:

	Signal<void(ErrorCode)> issueSet; /*!< Signal emitted when any issue is set. */

	/**
	 * @brief
	 * 
	 * @param moduleName 
	 * @param allowDebug 
	 * @param logFilePath 
	 */
	IssueHandler(
		const std::string& moduleName,
		bool allowDebug = false
	);

	/**
	 * @brief
	 * 
	 * @param errorCode 
	 * @param errorMessageToDisplay 
	 * @param detailedErrorMessage 
	 * @param showAsPopup 
	 */
	void SetIssue(
		ErrorCode errorCode,
		const std::string& errorMessageToDisplay,
		const std::string& detailedErrorMessage,
		IssueType issueType,
		IssueResolvable issueResolvable,
		bool showAsPopup = false
	);

	/**
	 * @brief
	 * 
	 * @param errorCode 
	 */
	void ResolveIssue(ErrorCode errorCode);

	/**
	 * @brief Note that if you destruct your callback i.e. a member function, you need to manually unsubscribe.
	 * 
	 * @param errorToSubscribeResolveOn 
	 * @param issueResolvedCallback
	 */
	void SubscribeToIssueSet(
		ErrorCode errorToSubscribeResolveOn,
		const std::function<void()>& issueResolvedCallback
	);

	/**
	 * @brief Note that if you destruct your callback i.e. a member function, you need to manually unsubscribe.
	 * 
	 * @param errorToSubscribeResolveOn 
	 * @param issueResolvedCallback
	 */
	void SubscribeToIssueResolved(
		ErrorCode errorToSubscribeResolveOn,
		const std::function<void()>& issueResolvedCallback
	);

	/**
	 * @brief 
	 * 
	 * @param errorToUnsubscribeResolveOn 
	 * @param issueResolvedCallback 
	 */
	void UnsubscribeIssueSet(
		ErrorCode errorToUnsubscribeResolveOn,
		const std::function<void()>& issueResolvedCallbackToRemove
	);

	/**
	 * @brief 
	 * 
	 * @param errorToUnsubscribeResolveOn 
	 * @param issueResolvedCallback 
	 */
	void UnsubscribeIssueResolved(
		ErrorCode errorToUnsubscribeResolveOn,
		const std::function<void()>& issueResolvedCallbackToRemove
	);

	/**
	 * @brief 
	 * 
	 * @param errorCode 
	 * @return
	 */
	bool IsIssueActive(ErrorCode errorCode);

};
