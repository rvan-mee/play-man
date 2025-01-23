#include "Issue.hpp"

bool operator == (const Issue& lhs, const Issue& rhs)
{
	return (lhs.errorCode == rhs.errorCode &&
			lhs.moduleName == rhs.moduleName &&
			lhs.errorMessage == rhs.errorMessage &&
			lhs.detailedErrorMessage == rhs.detailedErrorMessage &&
			lhs.showAsPopup == rhs.showAsPopup
	);
}

bool operator != (const Issue& lhs, const Issue& rhs)
{
	return !(lhs == rhs);
}

bool operator < (const Issue& lhs, const Issue& rhs)
{
	return lhs.errorCode < rhs.errorCode;
}
