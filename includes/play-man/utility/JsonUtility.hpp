#pragma once

#include <nlohmann/json.hpp>

#include <optional>

namespace Utility { namespace Json {

	constexpr size_t numberOfSpacesIndentation = 4; /* The number of spaces to use when indenting for serialization. */

	template <class T>
	std::optional<T> GetOptional(
		const nlohmann::json& jsonTree,
		const nlohmann::json::json_pointer& pathToValue)
	{
		try
		{
			return jsonTree.at(pathToValue);
		}
		catch(const std::exception&)
		{
			std::nullopt;
		}
	}

} /* namespace Json */

} /* namespace Utility */
