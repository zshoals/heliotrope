#pragma once

#include "varia/math/MathConstants.hpp"
#include <math.h>

constexpr VARIA_INLINE float vmath_lerp(float value, float start, float end)
{
	return (start * (1 - value)) + (end * value);
}

constexpr VARIA_INLINE float vmath_linear_norm(float value, float start, float end)
{
	return (value - start) / (end - start);
}

constexpr VARIA_INLINE float vmath_remap(float value, float s1, float e1, float s2, float e2)
{
	return vmath_linear_norm(vmath_lerp(value, s1, e1), s2, e2);
}

constexpr VARIA_INLINE float vmath_rad2deg(float radians)
{
	return static_cast<float>(radians * (180.0 / VARIA_PI));
}

constexpr VARIA_INLINE float vmath_deg2rad(float degrees)
{
	return static_cast<float>(degrees * (VARIA_PI / 180.0));
}

constexpr VARIA_INLINE float vmath_clampF(float value, float lower, float upper)
{
	if (value < lower) value = lower;
	if (value > upper) value = upper;

	return value;
}

constexpr VARIA_INLINE int vmath_clamp(int value, int lower, int upper)
{
	if (value < lower) value = lower;
	if (value > upper) value = upper;

	return value;
}

constexpr VARIA_INLINE float vmath_pow2(float value)
{
	return value * value;
}

template <typename T>
constexpr VARIA_INLINE T vmath_mod(T a, T b)
{
	return (a % b + b) % b;
}

template <typename T>
constexpr VARIA_INLINE T vmath_ceil_next_nearest(T value, T target_multiple)
{
	bool round_up = (value % target_multiple == 0) ? false : true;
	return (((target_multiple - (value % target_multiple)) * round_up) + value);
}

constexpr VARIA_INLINE unsigned int vmath_pow2_to_bitshift_value(size_t pow2_value)
{
	unsigned int shift = 0;
	while (pow2_value > 1)
	{
		pow2_value /= 2;
		++shift;
	}

	return shift;
}

constexpr VARIA_INLINE size_t vmath_pow2_next_nearest(size_t value)
{
	--value;
	value |= (value >> 1);
	value |= (value >> 2);
	value |= (value >> 4);
	value |= (value >> 8);
	value |= (value >> 16);
	++value;

	return value;
}
