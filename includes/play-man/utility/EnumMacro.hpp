#pragma once

#include <exception>

#define CREATE_ENUM_SWITCH_CASE_ENTRY(EnumName, name) case EnumName::name: return #name;

#define CREATE_ENUM_ENTRY(EnumName, name) name,

// Macro to define an enum and a string conversion function
#define DEFINE_ENUM_WITH_STRING(EnumName, ENUM) \
\
enum class EnumName \
{ \
	ENUM(CREATE_ENUM_ENTRY, EnumName) \
}; \
\
std::string_view EnumToString(EnumName value) \
{ \
	switch (value) \
	{ \
		ENUM(CREATE_ENUM_SWITCH_CASE_ENTRY, EnumName) \
		default: \
			throw std::runtime_error("Could not find value in " #EnumName); \
	} \
}
