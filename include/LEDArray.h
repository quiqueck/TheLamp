#ifndef __LEDArray__h__
#define __LEDArray__h__
#include "LEDViews.h"

class LEDArray : public LEDView1{
    public:
        LEDArray(CRGB** stores, float** alphas, uint16_t pixelCount);
        
        virtual CRGB &operator[](int i);
        virtual CRGB& pixel(int i);   
        virtual CRGB& pixelFromLayer(int i, LayerTypes layer); 
        virtual float& alphaFromLayer(int i, LayerTypes layer); 
    private:
        LEDArray(CRGB* store, uint16_t pixelCount);
        CRGB * const leds[LayerTypeCount];
        float * const alphas[LayerTypeCount];
};
#endif