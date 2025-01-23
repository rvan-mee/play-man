
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
	bool showAsPopup = false)
{

}

