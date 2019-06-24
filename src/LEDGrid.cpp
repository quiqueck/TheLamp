#include "LEDGrid.h"

LEDGrid::LEDGrid(std::pair<CRGB**, float**> store, uint16_t pixelCount, uint16_t pixelWidth, uint16_t pixelHeight, Origin o):
LEDArray(store.first, store.second, pixelCount), LEDView2(pixelWidth, pixelHeight), origin(o) {

}

LEDGridRowMajor::LEDGridRowMajor(std::pair<CRGB**, float**> store, uint16_t pixelWidth, uint16_t pixelHeight, Origin o) : 
    LEDGrid(store, pixelWidth*pixelHeight, pixelWidth, pixelHeight, o) {

}

float& LEDGridRowMajor::alphaFromLayer(int c, int r, LayerTypes layer){
    const uint8_t l = origin & oL;
    const uint8_t t = (origin & oT) > 1;
    r = (t)*(r) + (1-t)*((height-1) - r);

    const uint8_t a = abs(abs(l-t) - abs((1-t) - (r%2)));
    c = (a)*((width-1) - c) + (1-a)*(c);
    return LEDArray::alphaFromLayer(r*width + c, layer);
}

CRGB& LEDGridRowMajor::pixelFromLayer(int c, int r, LayerTypes layer){
    const uint8_t l = origin & oL;
    const uint8_t t = (origin & oT) > 1;
    r = (t)*(r) + (1-t)*((height-1) - r);

    const uint8_t a = abs(abs(l-t) - abs((1-t) - (r%2)));
    c = (a)*((width-1) - c) + (1-a)*(c);
    return LEDArray::pixelFromLayer(r*width + c, layer);
}

CRGB& LEDGridRowMajor::pixel(int c, int r){
    const uint8_t l = origin & oL;
    const uint8_t t = (origin & oT) > 1;
    r = (t)*(r) + (1-t)*((height-1) - r);

    const uint8_t a = abs(abs(l-t) - abs((1-t) - (r%2)));
    c = (a)*((width-1) - c) + (1-a)*(c);
    /*if (origin & oB == oB) {
        r = (height-1) - r;
        // a = abs(abs(l-t)-r%2)
        if (r%2 == 1) {            
            if (origin & oR == oR) { c = (width-1) - c; };      //l==0, t==0  a = 1
            //else c = c;                                       //l==1, t==0  a = 0
        } else {
            if (origin & oL == oL) { c = (width-1) - c; };      //l==1, t==0  a = 1
            //else c = c;                                       //l==0, t==0  a = 0
        }
    } else {
        if (r%2 == 1) {
            if (origin & oL == oL) { c = (width-1) - c; };      //l==1, t==1  a = 1
            //else c = c;                                       //l==0, t==1  a = 0
        } else {
            if (origin & oR == oR) { c = (width-1) - c; };      //l==0, t==1  a = 1
            //else c = c;                                       //l==1, t==1  a = 0
        }
    }*/

    return LEDArray::pixel(r*width + c);
}


LEDGridColumnMajor::LEDGridColumnMajor(std::pair<CRGB**, float**> store, uint16_t pixelWidth, uint16_t pixelHeight, Origin o) : 
    LEDGrid(store, pixelWidth*pixelHeight, pixelWidth, pixelHeight, o) {
}

float& LEDGridColumnMajor::alphaFromLayer(int c, int r, LayerTypes layer){
    const uint8_t l = origin & oL;
    const uint8_t t = (origin & oT) > 1;
    c = (l)*(c) + (1-l)*((width-1) - c);
    const uint8_t a = abs(abs(l-t) - abs((1-l) - (c%2)));
    r = (a)*((height-1) - r) + (1-a)*(r);

    return LEDArray::alphaFromLayer(r + c*height, layer);
}

CRGB& LEDGridColumnMajor::pixelFromLayer(int c, int r, LayerTypes layer){
    const uint8_t l = origin & oL;
    const uint8_t t = (origin & oT) > 1;
    c = (l)*(c) + (1-l)*((width-1) - c);
    const uint8_t a = abs(abs(l-t) - abs((1-l) - (c%2)));
    r = (a)*((height-1) - r) + (1-a)*(r);

    return LEDArray::pixelFromLayer(r + c*height, layer);
}

CRGB& LEDGridColumnMajor::pixel(int c, int r){
    const uint8_t l = origin & oL;
    const uint8_t t = (origin & oT) > 1;
    c = (l)*(c) + (1-l)*((width-1) - c);
    const uint8_t a = abs(abs(l-t) - abs((1-l) - (c%2)));
    r = (a)*((height-1) - r) + (1-a)*(r);

    return LEDArray::pixel(r + c*height);
}
