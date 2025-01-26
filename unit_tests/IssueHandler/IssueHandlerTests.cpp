
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/issueHandler/IssueHandler.hpp"
#include "play-man/logger/Logger.hpp"

TEST_CASE("Setting an issue")
{
	Logger::LogInterface::GetInstance().EnableCoutLogging(false);
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

TEST_CASE("Unsubscribe issue resolved signal")
{
	IssueHandler issueHandler("test");

	bool wasCallbackCalled = false;

	const auto callback = [&]()
	{
		wasCallbackCalled = true;
	};

	issueHandler.SubscribeToIssueResolved(
		ErrorCode::AnErrorCodeForTestingPurposes,
		callback
	);

	issueHandler.SetIssue(
		ErrorCode::AnErrorCodeForTestingPurposes,
		"test error",
		"setting an issue for testing purposes",
		IssueType::Error,
		IssueResolvable::AutomaticResolvable
	);

	issueHandler.UnsubscribeIssueResolved(
		ErrorCode::AnErrorCodeForTestingPurposes,
		callback
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);

	REQUIRE(!wasCallbackCalled);
}

TEST_CASE("Issue set signal")
{
	IssueHandler issueHandler("test");

	bool wasSignalEmitted = false;

	issueHandler.issueSet.Connect([&](ErrorCode ec)
	{
		wasSignalEmitted = ec == ErrorCode::AnErrorCodeForTestingPurposes;
	});

	issueHandler.SetIssue(
		ErrorCode::AnErrorCodeForTestingPurposes,
		"test error",
		"setting an issue for testing purposes",
		IssueType::Error,
		IssueResolvable::AutomaticResolvable
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);

	REQUIRE(wasSignalEmitted);
}
