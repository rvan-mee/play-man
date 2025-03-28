// ****************************************************************************** //
//   _______   __                              __       __                        //
//  /       \ /  |                            /  \     /  |                       //
//  $$$$$$$  |$$ |  ______   __    __         $$  \   /$$ |  ______   _______     //
//  $$ |__$$ |$$ | /      \ /  |  /  | ______ $$$  \ /$$$ | /      \ /       \    //
//  $$    $$/ $$ | $$$$$$  |$$ |  $$ |/      |$$$$  /$$$$ | $$$$$$  |$$$$$$$  |   //
//  $$$$$$$/  $$ | /    $$ |$$ |  $$ |$$$$$$/ $$ $$ $$/$$ | /    $$ |$$ |  $$ |   //
//  $$ |      $$ |/$$$$$$$ |$$ \__$$ |        $$ |$$$/ $$ |/$$$$$$$ |$$ |  $$ |   //
//  $$ |      $$ |$$    $$ |$$    $$ |        $$ | $/  $$ |$$    $$ |$$ |  $$ |   //
//  $$/       $$/  $$$$$$$/  $$$$$$$ |        $$/      $$/  $$$$$$$/ $$/   $$/    //
//                          /  \__$$ |                                            //
//                          $$    $$/                                             //
//                           $$$$$$/                                              //
//                                                                                //
//                            By: K1ngmar and rvan-mee                            //
// ****************************************************************************** //

#pragma once

#include <iostream>
#include <exception>
#include <string_view>

#define __CREATE_DEFAULT_ENUM(enum_class_name, enum_value_name) enum_value_name,
#define __CREATE_DEFINED_ENUM(enum_class_name, enum_value_name, value) enum_value_name = value,

#define __DEFAULT_ENUM_TO_STRING_CASE(enum_class_name, enum_value_name) case enum_class_name::enum_value_name: return #enum_value_name;
#define __DEFINED_ENUM_TO_STRING_CASE(enum_class_name, enum_value_name, value) case enum_class_name::enum_value_name: return #enum_value_name; 

/**
 * How __GET_CORRECT_MACRO works is it fills the _1 _2 arguments with the given values (enum_class_name and enum_value_name)
 * If the macro gets called with a default value _3 will be filled with the macro of the defined macro and the CORRECT_MACRO
 * Will be filled with the DEFAULT_MACRO. If there is a defined enum the value will be set inside _3 and CORRECT_MACRO
 * Will be filled with the DEFINED_MACRO. The extra ", 0" is there to ensure that there is at least one argument for the variadic parameter.
 */
#define __GET_CORRECT_MACRO(_1,_2,_3,CORRECT_MACRO,...) CORRECT_MACRO
#define __ENUM_CREATION(...) __GET_CORRECT_MACRO(__VA_ARGS__, __CREATE_DEFINED_ENUM, __CREATE_DEFAULT_ENUM, 0)(__VA_ARGS__)
#define __ENUM_TO_STRING_CASE(...) __GET_CORRECT_MACRO(__VA_ARGS__, __DEFINED_ENUM_TO_STRING_CASE, __DEFAULT_ENUM_TO_STRING_CASE, 0)(__VA_ARGS__)

/**
 * @brief Defines an enum with optionally specified values, and some utility functions like to string for the enum,
 *        For more information as to how this works you should look up the term `x-macro`.
 * 
 * @param ENUM_DEFINITION   This is the sequence of enum entries (which itself is a macro).
 *                          The ENUM_DEFINITION itself needs to be defined as `ENUM_DEFINITION(X, enum_class_name)`.
 *                          Each entry needs to be defined like so `X(enum_name, enum_entry_name, value)` where the value is optional.
 * 
 * @param enum_class_name
 */
#define CREATE_ENUM_WITH_UTILS(ENUM_DEFINITION, enum_class_name)\
\
enum class enum_class_name {\
    ENUM_DEFINITION(__ENUM_CREATION, enum_class_name)\
}; \
\
inline std::string_view GetEnumAsString(enum_class_name val)\
{\
    switch (val)\
    {\
        ENUM_DEFINITION(__ENUM_TO_STRING_CASE, enum_class_name)\
        default:\
			throw (std::runtime_error("Unknown enum value for enum: " #enum_class_name + std::to_string(static_cast<uint32_t>(val))));\
    }\
}\
inline int32_t GetEnumAsValue(enum_class_name val)\
{\
    return (static_cast<int32_t>(val));\
}\
inline std::ostream& operator << (std::ostream& os, const enum_class_name& val)\
{\
    os << GetEnumAsString(val);\
    return (os);\
}
