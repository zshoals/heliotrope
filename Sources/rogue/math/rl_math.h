#pragma once

#include "rogue/rl_common.h"
#include "rogue/math/rl_math_constants.h"
#include <math.h>

RL_INLINE double rl_math_lerp_d(double value, double start, double end)
{
    return (start * (1.0 - value)) + (end * value);
}

RL_INLINE double rl_math_linear_norm_d(double value, double start, double end)
{
    return (value - start) / (end - start);
}

RL_INLINE double rl_math_remap_d(double value, double s1, double e1, double s2, double e2)
{
    return rl_math_linear_norm_d(rl_math_lerp_d(value, s1, e1), s2, e2);
}

RL_INLINE double rl_math_rad2deg_d(double radians)
{
    return (radians * (180.0 / RL_MATH_CONSTANT_PI));
}

RL_INLINE double rl_math_deg2rad_d(double degrees)
{
    return (degrees * (RL_MATH_CONSTANT_PI / 180.0));
}

RL_INLINE double rl_math_clamp_d(double value, double lower, double upper)
{
    if (value < lower) { value = lower; }
    if (value > upper) { value = upper; }

    return value;
}

RL_INLINE double rl_math_pow2_d(double value)
{
    return value * value;
}

RL_INLINE int rl_math_clamp_i(int value, int lower, int upper)
{
    if (value < lower) { value = lower; }
    if (value > upper) { value = upper; }

    return value;
}
