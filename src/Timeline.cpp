#include "Timeline.h"
#include "Animation.h"

#include <Arduino.h>



Timeline::Timeline(){
    time = -1;
    lastTick = 0;
    accDelta = 0;
}

void Timeline::reset(){
    lastTick = millis();
    time = 0;
    accDelta = 0;
}

void Timeline::tick(){
    if (!active) return;
    
    const unsigned long now = millis();
    const unsigned long delta = (now - lastTick);
    time += delta / 1000.0f;
    accDelta += delta;    
    lastTick = now;

    if (accDelta >= 1000/FPS) {
        accDelta -= 1000/FPS;
        //Serial.printf("Time: %fs\n", time);
    }
}