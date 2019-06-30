#include "LEDArray.h"


LEDArray::LEDArray(CRGB* store, uint16_t pixelCount) : 
    LEDView1(pixelCount), 
    leds{store, store, store, store, store},
    alphas{nullptr, nullptr, nullptr, nullptr, nullptr}
{ 
    Serial.printf("INIT1 %x\n",  (void*)(&store[0]));
    //nothing to do here, init is done in begin()
}

LEDArray::LEDArray(CRGB** stores, float** alphaIns, uint16_t pixelCount) : 
    LEDView1(pixelCount), 
    leds{stores[0], stores[1], stores[2], stores[3], stores[4], stores[5]},
    alphas{alphaIns[0], alphaIns[1], alphaIns[2], alphaIns[3], alphaIns[4], alphaIns[5]}
{ 
    Serial.printf("INIT2 %x, %x, %x\n",  (void*)(&stores[0][0]), stores, stores[0]);

    //nothing to do here, init is done in begin()
}
CRGB dummy = CRGB::Red;

CRGB& LEDArray::pixel(int i){
    if (i==0)
        Serial.printf("PIXEL %d: %x\n", i, (void*)(&leds[0][0]));
    
    return leds[LEDArray::FinalComposit][i];
}

CRGB& LEDArray::pixelFromLayer(int i, LayerTypes layer){
    return leds[layer][i];
}

float& LEDArray::alphaFromLayer(int i, LayerTypes layer){
    return alphas[layer][i];
}

CRGB& LEDArray::operator[](int i) {
    i %= count;
    while (i<0) i += count; 

    return leds[FinalComposit][i];
}