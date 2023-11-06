#pragma once

#include "rogue/rl_common.h"

RL_INLINE int64_t rl_string_tools_string_to_integer(char const * string, int64_t length)
{
    if (length >= 20) { return 0; }

    int64_t power = 1;
    int64_t result = 0;
    for (int64_t i = length - 1; i >= 0; i -= 1)
    {
        result += (string[i] - '0') * power;
        power *= 10;
    }

    return result;
}