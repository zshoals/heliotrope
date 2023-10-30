#pragma once

#include "varia/vcommon.hpp"
#include <math.h>

//Note(zshoals Dec-11-2022): Attribution to https://github.com/mnemocron/easing-functions

constexpr double PI = 3.145926545;

VARIA_INLINE float vmath_easeInSine( float t ) {
    return 1 + sin( 1.5707963 * (--t) );
}

VARIA_INLINE float vmath_easeOutSine( float t ) {
    return sin( 1.5707963 * t );
}

VARIA_INLINE float vmath_easeInOutSine( float t ) {
    return 0.5 * (1 + sin( 3.1415926 * (t - 0.5) ) );
}

VARIA_INLINE float vmath_easeInQuad( float t ) {
    return t * t;
}

VARIA_INLINE float vmath_easeOutQuad( float t ) {
    return t * (2 - t);
}

VARIA_INLINE float vmath_easeInOutQuad( float t ) {
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

VARIA_INLINE float vmath_easeInCubic( float t ) {
    return t * t * t;
}

VARIA_INLINE float vmath_easeOutCubic( float t ) {
    return 1 + (--t) * t * t;
}

VARIA_INLINE float vmath_easeInOutCubic( float t ) {
    //return t < 0.5 ? 4 * t * t * t : 1 + (t) * (2 * (--t)) * (2 * t);
    // return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);  // returns false output
    return t < 0.5 ? 4 * t * t * t : (t-1)*(2*t-2)*(2*t-2)+1;
}

VARIA_INLINE float vmath_easeInQuart( float t ) {
    t *= t;
    return t * t;
}

VARIA_INLINE float vmath_easeOutQuart( float t ) {
    t = (--t) * t;
    return 1 - t * t;
}

VARIA_INLINE float vmath_easeInOutQuart( float t ) {
    if( t < 0.5 ) {
        t *= t;
        return 8 * t * t;
    } else {
        t = (--t) * t;
        return 1 - 8 * t * t;
    }
}

VARIA_INLINE float vmath_easeInQuint( float t ) {
    float t2 = t * t;
    return t * t2 * t2;
}

VARIA_INLINE float vmath_easeOutQuint( float t ) {
    float t2 = (--t) * t;
    return 1 + t * t2 * t2;
}

VARIA_INLINE float vmath_easeInOutQuint( float t ) {
    float t2;
    if( t < 0.5 ) {
        t2 = t * t;
        return 16 * t * t2 * t2;
    } else {
        t2 = (--t) * t;
        return 1 + 16 * t * t2 * t2;
    }
}

VARIA_INLINE float vmath_easeInExpo( float t ) {
    return (pow( 2, 8 * t ) - 1) / 255;
}

VARIA_INLINE float vmath_easeOutExpo( float t ) {
    return 1 - pow( 2, -8 * t );
}

VARIA_INLINE float vmath_easeInOutExpo( float t ) {
    if( t < 0.5 ) {
        return (pow( 2, 16 * t ) - 1) / 510;
    } else {
        return 1 - 0.5 * pow( 2, -16 * (t - 0.5) );
    }
}

VARIA_INLINE float vmath_easeInCirc( float t ) {
    return 1 - sqrt( 1 - t );
}

VARIA_INLINE float vmath_easeOutCirc( float t ) {
    return sqrt( t );
}

VARIA_INLINE float vmath_easeInOutCirc( float t ) {
    if( t < 0.5 ) {
        return (1 - sqrt( 1 - 2 * t )) * 0.5;
    } else {
        return (1 + sqrt( 2 * t - 1 )) * 0.5;
    }
}

VARIA_INLINE float vmath_easeInBack( float t ) {
    return t * t * (2.70158 * t - 1.70158);
}

VARIA_INLINE float vmath_easeOutBack( float t ) {
    return 1 + (--t) * t * (2.70158 * t + 1.70158);
}

VARIA_INLINE float vmath_easeInOutBack( float t ) {
    if( t < 0.5 ) {
        return t * t * (7 * t - 2.5) * 2;
    } else {
        return 1 + (--t) * t * 2 * (7 * t + 2.5);
    }
}

VARIA_INLINE float vmath_easeInElastic( float t ) {
    float t2 = t * t;
    return t2 * t2 * sin( t * PI * 4.5 );
}

VARIA_INLINE float vmath_easeOutElastic( float t ) {
    float t2 = (t - 1) * (t - 1);
    return 1 - t2 * t2 * cos( t * PI * 4.5 );
}

VARIA_INLINE float vmath_easeInOutElastic( float t ) {
    float t2;
    if( t < 0.45 ) {
        t2 = t * t;
        return 8 * t2 * t2 * sin( t * PI * 9 );
    } else if( t < 0.55 ) {
        return 0.5 + 0.75 * sin( t * PI * 4 );
    } else {
        t2 = (t - 1) * (t - 1);
        return 1 - 8 * t2 * t2 * sin( t * PI * 9 );
    }
}

VARIA_INLINE float vmath_easeInBounce( float t ) {
    return pow( 2, 6 * (t - 1) ) * fabsf( sin( t * PI * 3.5 ) );
}

VARIA_INLINE float vmath_easeOutBounce( float t ) {
    return 1 - pow( 2, -6 * t ) * fabsf( cos( t * PI * 3.5 ) );
}

VARIA_INLINE float vmath_easeInOutBounce( float t ) {
    if( t < 0.5 ) {
        return 8 * pow( 2, 8 * (t - 1) ) * fabsf( sin( t * PI * 7 ) );
    } else {
        return 1 - 8 * pow( 2, -8 * t ) * fabsf( sin( t * PI * 7 ) );
    }
}