
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/issueHandler/IssueHandler.hpp"
#include "play-man/logger/Logger.hpp"

namespace TestFixtures
{

	/**
	 * @brief Fixture for testing the issueHandler.
	 */
	struct IssueHandlerFixture
	{
		IssueHandler issueHandler;

		IssueHandlerFixture() : issueHandler("test")
		{
			Logger::LogInterface::Initialize("test", Logger::LogLevel::Debug);
		}

		~IssueHandlerFixture()
		{
			Logger::LogInterface::GetInstance().reset();
		}
	};

} /* namespace TestFixtures*/

TEST_CASE_METHOD(TestFixtures::IssueHandlerFixture, "Setting an issue")
{
	REQUIRE(!issueHandler.IsIssueActive(ErrorCode::AnErrorCodeForTestingPurposes));

	issueHandler.SetIssue(
		ErrorCode::AnErrorCodeForTestingPurposes,
		"test error",
		"setting an issue for testing purposes",
		IssueType::Error,
		IssueResolvable::AutomaticallyResolvable
	);

	REQUIRE(issueHandler.IsIssueActive(ErrorCode::AnErrorCodeForTestingPurposes));
}

TEST_CASE_METHOD(TestFixtures::IssueHandlerFixture, "Resolving an issue")
{
	issueHandler.SetIssue(
		ErrorCode::AnErrorCodeForTestingPurposes,
		"test error",
		"setting an issue for testing purposes",
		IssueType::Error,
		IssueResolvable::AutomaticallyResolvable
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);
}

TEST_CASE_METHOD(TestFixtures::IssueHandlerFixture, "Issue resolved signal")
{
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
		IssueResolvable::AutomaticallyResolvable
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);

	REQUIRE(wasCallbackCalled);
}

TEST_CASE_METHOD(TestFixtures::IssueHandlerFixture, "Unsubscribe issue resolved signal")
{
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
		IssueResolvable::AutomaticallyResolvable
	);

	issueHandler.UnsubscribeIssueResolved(
		ErrorCode::AnErrorCodeForTestingPurposes,
		callback
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);

	REQUIRE(!wasCallbackCalled);
}

TEST_CASE_METHOD(TestFixtures::IssueHandlerFixture, "Issue set signal")
{
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
		IssueResolvable::AutomaticallyResolvable
	);

	issueHandler.ResolveIssue(ErrorCode::AnErrorCodeForTestingPurposes);

	REQUIRE(wasSignalEmitted);
}
