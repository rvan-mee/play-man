#pragma once

#include "ErrorCodes.hpp"
#include "Issue.hpp"

#include <string>
#include <optional>
#include <map>
#include <memory>
#include <functional>
#include <set>

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
class IssueHandler
{
	using IssueResolvedCallbackType = std::weak_ptr<std::function<void()> >;

	const std::string moduleName; /*! */
	const bool allowDebug; /*!< -. */
	const std::optional<std::string> logFilePath; /*! */
	std::multimap<ErrorCode, IssueResolvedCallbackType> issueResolvedCallbacks; /*!  */
	std::set<Issue> activeErrors; /*!< -. */

public:

	/**
	 * @brief
	 * 
	 * @param moduleName 
	 * @param allowDebug 
	 * @param logFilePath 
	 */
	IssueHandler(
		const std::string& moduleName,
		bool allowDebug = false,
		std::optional<std::string> logFilePath = std::nullopt
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
		std::weak_ptr<IssueResolvedCallbackType> issueResolvedCallback
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
