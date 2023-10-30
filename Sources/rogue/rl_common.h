#pragma once

//Always include C's extended scalar types
#include <stdint.h>

#ifdef _MSC_VER
	#define RL_INLINE static __forceinline
#else
	#define RL_INLINE static __attribute__((always_inline))
#endif

#define RL_NO_OPERATION() ((void)0)

