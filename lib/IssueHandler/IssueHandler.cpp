
#include "play-man/IssueHandler/IssueHandler.hpp"

IssueHandler::IssueHandler(
	const std::string& moduleName_,
	bool allowDebug_,
	std::optional<std::string> logFilePath_)
	: moduleName(moduleName_)
	, allowDebug(allowDebug_)
	, logFilePath(logFilePath_)
{
	
}

void IssueHandler::SetIssue(
	ErrorCode errorCode,
	const std::string& errorMessageToDisplay,
	const std::string& detailedErrorMessage,
	IssueType issueType,
	IssueResolvable issueResolvable,
	bool showAsPopup)
{
	activeErrors.emplace(
		errorCode,
		Issue{
			errorCode,
			issueType,
			issueResolvable,
			moduleName,
			errorMessageToDisplay,
			detailedErrorMessage,
			showAsPopup
		}
	);

	issueSet(errorCode);
}

void IssueHandler::ResolveIssue(ErrorCode errorCode)
{
	if (!IsIssueActive(errorCode))
	{
		return;
	}

	activeErrors.erase(errorCode);

	auto issueResolvedCallbackItr = issueResolvedCallbacks.find(errorCode);
	if (issueResolvedCallbackItr != issueResolvedCallbacks.end())
	{
		issueResolvedCallbackItr->second();
	}
}

void IssueHandler::SubscribeToIssueResolved(
	ErrorCode errorToSubscribeResolveOn,
	const std::function<void()>& issueResolvedCallback)
{
	issueResolvedCallbacks[errorToSubscribeResolveOn].Connect(issueResolvedCallback);
}

bool IssueHandler::IsIssueActive(ErrorCode errorCode)
{
	return activeErrors.contains(errorCode);
}
