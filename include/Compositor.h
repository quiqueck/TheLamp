#ifndef __COMPOSITOR_H__
#define __COMPOSITOR_H__
#include "LEDViews.h"

typedef std::function<CRGB (const CRGB&, const CRGB&, float)> BlendFunction;
inline CRGB keepBlend(const CRGB& c0, const CRGB& c1, float a){
    return c0;
}

inline CRGB overBlend(const CRGB& c0, const CRGB& c1, float a){
    return c1;
}

inline CRGB alphaBlend(const CRGB& c0, const CRGB& c1, float a){
    const float aa = 1.0f-a;
    return CRGB(c1.r * a + c0.r * aa, c1.g * a + c0.g * aa, c1.b * a + c0.b * aa);
}

#endif