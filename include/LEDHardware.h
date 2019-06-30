#ifndef __LEDHardware__h__
#define __LEDHardware__h__
#include "LEDArray.h"

#define NUMPIXELS 300

class LEDHardware : public LEDArray {
    public:
        LEDHardware();
        virtual bool begin();

    
        std::pair<CRGB**, float**> storeAt(int idx) { 
            CRGB** s = new CRGB*[LayerTypeCount];
            float** a = new float*[LayerTypeCount];

            for (uint8_t i=0; i<LayerTypeCount; i++){
                s[i] = &pixelFromLayer(idx, (LayerTypes)i);
                a[i] = &alphaFromLayer(idx, (LayerTypes)i);
            }
            return std::make_pair(s, a);
        }
        inline void show() { FastLED.show(); }
};
#endif