#include <iostream>

#define CREATE_DEFAULT_ENUM(enum_class_name, enum_value_name) enum_value_name,
#define CREATE_DEFINED_ENUM(enum_class_name, enum_value_name, value) enum_value_name = value,

#define DEFAULT_ENUM_TO_STRING_CASE(enum_class_name, enum_value_name) case enum_class_name::enum_value_name: return #enum_value_name;
#define DEFINED_ENUM_TO_STRING_CASE(enum_class_name, enum_value_name, value) case enum_class_name::enum_value_name: return #enum_value_name; 

/**
 * How GET_CORRECT_MACRO works is it fills the _1 _2 arguments with the given values (enum_class_name and enum_value_name)
 * If the macro gets called with a default value _3 will be filled with the macro of the defined macro and the CORRECT_MACRO_NAME
 * Will be filled with the DEFAULT_MACRO. If there is a defined enum the value will be set inside _3 and CORRECT_MACRO_NAME
 * Will be filled with the DEFINED_MACRO. The extra ", 0" is there to ensure that there is at least one argument for the variadic parameter.
 */
#define GET_CORRECT_MACRO(_1,_2,_3,CORRECT_MACRO_NAME,...) CORRECT_MACRO_NAME
#define ENUM_CREATION(...) GET_CORRECT_MACRO(__VA_ARGS__, CREATE_DEFINED_ENUM, CREATE_DEFAULT_ENUM, 0)(__VA_ARGS__)
#define ENUM_TO_STRING_CASE(...) GET_CORRECT_MACRO(__VA_ARGS__, DEFINED_ENUM_TO_STRING_CASE, DEFAULT_ENUM_TO_STRING_CASE, 0)(__VA_ARGS__)

#define CreateEnumAndUtils(ENUM_DEFINITION, enum_class_name)\
\
enum class enum_class_name {\
    ENUM_DEFINITION(ENUM_CREATION, enum_class_name)\
}; \
\
const char* getEnumAsString(enum_class_name val)\
{\
    switch (val)\
    {\
        ENUM_DEFINITION(ENUM_TO_STRING_CASE, enum_class_name)\
        default:\
            return ("");\
    }\
}\
inline int32_t getEnumAsValue(enum_class_name val)\
{\
    return (static_cast<int32_t>(val));\
}\
std::ostream& operator << (std::ostream& os, const enum_class_name& val) \
{\
    os << getEnumAsString(val);\
    return (os);\
}\


#define DEFAULT_ENUM_DEFINITION(EXPANSION_MACRO, enum_class_name) \
    EXPANSION_MACRO(enum_class_name, A) \
    EXPANSION_MACRO(enum_class_name, B) \
    EXPANSION_MACRO(enum_class_name, C) \
    EXPANSION_MACRO(enum_class_name, D)

CreateEnumAndUtils(DEFAULT_ENUM_DEFINITION, DefaultEnum);

#define COLOR_DEFINITION(EXPANSION_MACRO, enum_class_name) \
    EXPANSION_MACRO(enum_class_name, RED, 2) \
    EXPANSION_MACRO(enum_class_name, BLUE, 4) \
    EXPANSION_MACRO(enum_class_name, GREEN, 5) \
    EXPANSION_MACRO(enum_class_name, YELLOW, 15)

CreateEnumAndUtils(COLOR_DEFINITION, Color);

#define SECOND_COLOR_DEFINITION(EXPANSION_MACRO, enum_class_name) \
    EXPANSION_MACRO(enum_class_name, AQUA, 2) \
    EXPANSION_MACRO(enum_class_name, BLUE, 4) \
    EXPANSION_MACRO(enum_class_name, GREEN, 5) \
    EXPANSION_MACRO(enum_class_name, YELLOW, 25)

namespace second {
    CreateEnumAndUtils(SECOND_COLOR_DEFINITION, Color);
}

int main()
{
    std::cout << getEnumAsString(Color::YELLOW) << std::endl;
    std::cout << Color::YELLOW << std::endl;
    std::cout << getEnumAsValue(Color::YELLOW) << std::endl;
    std::cout << std::endl;

    std::cout << getEnumAsString(second::Color::YELLOW) << std::endl;
    std::cout << second::Color::YELLOW << std::endl;
    std::cout << getEnumAsValue(second::Color::YELLOW) << std::endl;
    std::cout << std::endl;

    std::cout << getEnumAsString(DefaultEnum::A) << " " << DefaultEnum::A << " " << getEnumAsValue(DefaultEnum::A) << std::endl;
    std::cout << getEnumAsString(DefaultEnum::B) << " " << DefaultEnum::B << " " << getEnumAsValue(DefaultEnum::B) << std::endl;
    std::cout << getEnumAsString(DefaultEnum::C) << " " << DefaultEnum::C << " " << getEnumAsValue(DefaultEnum::C) << std::endl;
    std::cout << getEnumAsString(DefaultEnum::D) << " " << DefaultEnum::D << " " << getEnumAsValue(DefaultEnum::D) << std::endl;

    return (0);
}
