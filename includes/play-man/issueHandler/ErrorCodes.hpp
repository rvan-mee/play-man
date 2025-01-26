#pragma once

#include "play-man/utility/EnumMacro.hpp"

#define ErrorCodeSeq(x, n) \
    x(n, AnErrorCodeForTestingPurposes)

/**
 * @brief -.
 */
CREATE_ENUM_WITH_UTILS(ErrorCodeSeq, ErrorCode)
#undef ErrorCodeSeq
