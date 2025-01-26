
#include "play-man/issueHandler/IssueHandler.hpp"
#include "play-man/logger/Logger.hpp"

IssueHandler::IssueHandler(
	const std::string& moduleName_,
	bool allowDebug_)
	: moduleName(moduleName_)
	, allowDebug(allowDebug_)
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

	const auto errorMessage =
		"Issue set (" + std::string(GetEnumAsString(issueResolvable)) + "): " +
		std::string(GetEnumAsString(errorCode)) + ".\n"
		"Error message: " + errorMessageToDisplay + ".\n"
		"Detailed error message: " + detailedErrorMessage;

	switch (issueType)
	{
		case IssueType::Error:
			LOG_ERROR(errorMessage);
			break;
		case IssueType::Warning:
			LOG_WARNING(errorMessage);
			break;
		case IssueType::Debug:
			LOG_DEBUG(errorMessage);
			break;
	}

	issueSet(errorCode);
}

void IssueHandler::ResolveIssue(ErrorCode errorCode)
{
	if (!IsIssueActive(errorCode))
	{
		return;
	}

	if (activeErrors.at(errorCode).issueResolvable == IssueResolvable::Fatal)
	{
		return;
	}

	activeErrors.erase(errorCode);

	LOG_INFO(std::string("Issue resolved: ") + GetEnumAsString(errorCode).data());

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

void IssueHandler::UnsubscribeIssueResolved(
	ErrorCode errorToUnsubscribeResolveOn,
	const std::function<void()>& issueResolvedCallbackToRemove)
{
	issueResolvedCallbacks.at(errorToUnsubscribeResolveOn).Disconnect(issueResolvedCallbackToRemove);
}

bool IssueHandler::IsIssueActive(ErrorCode errorCode)
{
	return activeErrors.contains(errorCode);
}
