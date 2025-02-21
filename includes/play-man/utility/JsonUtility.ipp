#include "JsonUtility.hpp"

template <class T>
std::optional<T> GetOptional(
	const nlohmann::json& jsonTree,
	const nlohmann::json::json_pointer& pathToValue)
{
	try
	{
		return jsonTree.at(pathToValue);
	}
	catch (const std::exception&)
	{
		return std::nullopt;
	}
}
