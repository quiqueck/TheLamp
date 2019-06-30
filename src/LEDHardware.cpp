#include <Arduino.h>
#include "LEDHardware.h"
#include "LEDViews.h"

static CRGB __b0[NUMPIXELS];
static CRGB __b1[NUMPIXELS];
static CRGB __b2[NUMPIXELS];
static CRGB __b3[NUMPIXELS];
static CRGB __b4[NUMPIXELS];
static CRGB __b5[NUMPIXELS];

static float __a0[NUMPIXELS];
static float __a1[NUMPIXELS];
static float __a2[NUMPIXELS];
static float __a3[NUMPIXELS];
static float __a4[NUMPIXELS];
static float __a5[NUMPIXELS];

//remeber to change LEDArray::LEDArray if more buffers are needed!!!

static CRGB* __leds[LEDState::LayerTypeCount] = {(CRGB*)__b0, (CRGB*)__b1, (CRGB*)__b2, (CRGB*)__b3, (CRGB*)__b4, (CRGB*)__b5};
static float* __ledsAlpha[LEDState::LayerTypeCount] = {(float*)__a0, (float*)__a1, (float*)__a2, (float*)__a3, (float*)__a4, (float*)__a5};

LEDHardware::LEDHardware(): LEDArray((CRGB**)__leds, (float**)__ledsAlpha, NUMPIXELS){ 
    // Serial.printf("ADD: %x\n",  (void*)(&__leds[0][0]));
    // for (int i=0; i<NUMPIXELS; i++){
    //     __leds[0][i] = CRGB::Blue;
    // }
}

bool LEDHardware::begin(){
    Serial.printf("Booting up FastLED using %d Pixels.\n\n", NUMPIXELS);

    FastLED.setBrightness(0xee);
    Serial.printf("LINK: %x\n",  (void*)(&(__leds[0][0])));
    FastLED.addLeds<SK9822, SPI_DATA_PIN, SPI_CLK_PIN, BGR, DATA_RATE_MHZ(12)>(__leds[0], NUMPIXELS);

    return true;
}