
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/IssueHandler/IssueHandler.hpp"

TEST_CASE("Setting an issue")
{
	IssueHandler issueHandler("test");

	REQUIRE(!issueHandler.IsIssueActive(ErrorCode::AnErrorCodeForTestingPurposes));

	issueHandler.SetIssue(
		ErrorCode::AnErrorCodeForTestingPurposes,
		"test error",
		"setting an issue for testing purposes",
		IssueType::Error,
		IssueResolvable::AutomaticResolvable
	);

	REQUIRE(issueHandler.IsIssueActive(ErrorCode::AnErrorCodeForTestingPurposes));
}

TEST_CASE("Resolving an issue")
{
	IssueHandler issueHandler("test");

	issueHandler.SetIssue(
		ErrorCode::AnErrorCodeForTestingPurposes,
		"test error",
		"setting an issue for testing purposes",
		IssueType::Error,
		IssueResolvable::AutomaticResolvable
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);
}

TEST_CASE("Issue resolved signal")
{
	IssueHandler issueHandler("test");

	bool wasCallbackCalled = false;

	issueHandler.SubscribeToIssueResolved(
		ErrorCode::AnErrorCodeForTestingPurposes,
		[&]()
		{
			wasCallbackCalled = true;
		}
	);

	issueHandler.SetIssue(
		ErrorCode::AnErrorCodeForTestingPurposes,
		"test error",
		"setting an issue for testing purposes",
		IssueType::Error,
		IssueResolvable::AutomaticResolvable
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);

	REQUIRE(wasCallbackCalled);
}