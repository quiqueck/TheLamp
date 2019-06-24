#ifndef __LEDGrid__h__
#define __LEDGrid__h__
#include "LEDArray.h"

class LEDGrid : public LEDArray, public LEDView2 {
    public:
        enum Origin { oTL = 0x11, oTR = 0x12, oBL = 0x21, oBR = 0x22, oT = 0x10, oB = 0x20, oL = 0x01, oR=0x02 };
        LEDGrid(std::pair<CRGB**, float**> store, uint16_t pixelCount, uint16_t pixelWidth, uint16_t pixelHeight, Origin o = oTL);        

        virtual CRGB& operator()(int x, int y) { return pixel(x, y); };

        const Origin origin;

        virtual CRGB& pixel(int x, int y) = 0;        
        virtual CRGB& pixelFromLayer(int x, int y, LayerTypes layer) = 0; 
        virtual float& alphaFromLayer(int x, int y, LayerTypes layer) = 0;     
};

class LEDGridRowMajor : public LEDGrid{
    public:
        LEDGridRowMajor(std::pair<CRGB**, float**>, uint16_t pixelWidth, uint16_t pixelHeight, Origin o);
        virtual CRGB& pixel(int x, int y);
        virtual CRGB& pixelFromLayer(int x, int y, LayerTypes layer);  
        virtual float& alphaFromLayer(int x, int y, LayerTypes layer);
};

class LEDGridColumnMajor : public LEDGrid{
    public:
        LEDGridColumnMajor(std::pair<CRGB**, float**>, uint16_t pixelWidth, uint16_t pixelHeight, Origin o);
        virtual CRGB& pixel(int x, int y);
        virtual CRGB& pixelFromLayer(int x, int y, LayerTypes layer); 
        virtual float& alphaFromLayer(int x, int y, LayerTypes layer);
};
#endif