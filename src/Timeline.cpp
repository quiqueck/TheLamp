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

void Timeline::runAt(double time){
    Serial.printf("Time %f\n", time);
    uint8_t layerMask = 0;
    tracks.runAt(time, [&layerMask](const double myTime, const AnimationList::TimeItem& item) { 
            addBit(layerMask, item.layer);
            item.value->render(myTime, item.layer);
    });

    Serial.printf(" => mask=%d\n", layerMask);
    compositors.runAt(time, [layerMask](const double myTime, const CompositList::TimeItem& item) { 
            item.value->composit(myTime, layerMask, LEDState::LayerTypes::SolidBackground, item.layer);
    });
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
        runAt(time);
        
        //Serial.printf("Time: %fs\n", time);
    }
}

void Timeline::addTrack(AnimationList::PItemType track, double startTime, double playbackDuration, LEDState::LayerTypes layer){
    tracks.addItem(track, startTime, playbackDuration, layer);
}

void Timeline::addCompositor(CompositList::PItemType track, double startTime, double playbackDuration, LEDState::LayerTypes layer){
    compositors.addItem(track, startTime, playbackDuration, layer);
}
