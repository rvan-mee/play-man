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
 * @brief
 */
class IssueHandler
{
	using IssueResolvedCallbackType = Signal<void()>;

	const std::string moduleName; /*! */
	const bool allowDebug; /*!< -. */
	std::map<ErrorCode, IssueResolvedCallbackType> issueResolvedCallbacks; /*!  */
	std::map<ErrorCode, Issue> activeErrors; /*!<  */

public:

	Signal<void(ErrorCode)> issueSet; /*!< Signal emitted when an issue is set. */

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
	 * @brief
	 * 
	 * @param errorToSubscribeResolveOn 
	 * @param issueResolvedCallback  The callback is a weak_ptr, so that raii can be used for member functions, be it not the most convenient.
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
	void UnsubscribeIssueResolved(
		ErrorCode errorToUnsubscribeResolveOn,
		const std::function<void()>& issueResolvedCallbackToRemove
	);

	/**
	 * @brief 
	 * 
	 * @param errorCode 
	 * @return true 
	 * @return false 
	 */
	bool IsIssueActive(ErrorCode errorCode);

};
