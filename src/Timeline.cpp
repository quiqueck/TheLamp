#include "Timeline.h"
#include "Animation.h"

#include <Arduino.h>

Tickable::Tickable(){
    active = false;
    time = -1;
    lastTick = 0;
    accDelta = 0;
}

void Tickable::resetTime(){
    lastTick = millis();
    time = 0;
    accDelta = 0;
}



bool Tickable::tick(bool earlyOut, bool callRun){
    if (earlyOut && !active) return false;
    
    const unsigned long now = millis();
    const unsigned long delta = (now - lastTick);
    if (active) time += delta / 1000.0f;
    accDelta += delta;    
    lastTick = now;

    if (accDelta >= 1000/FPS) {
        accDelta -= 1000/FPS;
        if (callRun) { 
            runAt(time);
            return true;
        }
    }
    return false;
}


Timeline::Timeline() : Tickable(){
    
}

void Timeline::reset(){
    //performs hard resets
    actions.reset();
    compositors.reset();
    tracks.reset();
    resetTime();
}

uint8_t Timeline::runWithTransition(){
    Serial.printf("Composit Time %f\n", time);
    uint8_t layerMask = 0;
    tracks.runAt(time, [&layerMask](const TimeFormat myTime, const AnimationList::TimeItem& item) { 
            addBit(layerMask, item.layer);
            item.value->render(myTime, item.layer);
    });

    return layerMask;
}

void Timeline::compositWithTransition(uint8_t layerMask, LEDState::LayerTypes Background, LEDState::LayerTypes Final){
    Serial.printf(" => mask=%d\n", layerMask);
    const LEDState::LayerTypes BG = Background;
    const LEDState::LayerTypes FG = Final;
    compositors.runAt(time, [layerMask, BG, FG](const TimeFormat myTime, const CompositList::TimeItem& item) { 
        item.value->composit(myTime, layerMask, BG, FG);
    });

    actions.runAt(time, [layerMask](const TimeFormat myTime, const ActionList::TimeItem& item) {             
            item.value->execute(myTime, layerMask, item.layer);
    });
}

void Timeline::runAt(TimeFormat time){
    Serial.printf("Time %f\n", time);
    uint8_t layerMask = 0;
    tracks.runAt(time, [&layerMask](const TimeFormat myTime, const AnimationList::TimeItem& item) { 
            addBit(layerMask, item.layer);
            item.value->render(myTime, item.layer);
    });

    Serial.printf(" => mask=%d\n", layerMask);
    compositors.runAt(time, [layerMask](const TimeFormat myTime, const CompositList::TimeItem& item) { 
            item.value->composit(myTime, layerMask, LEDState::LayerTypes::SolidBackground, item.layer);
    });

    actions.runAt(time, [layerMask](const TimeFormat myTime, const ActionList::TimeItem& item) {             
            item.value->execute(myTime, layerMask, item.layer);
    });
}

void Timeline::goToTime(TimeFormat t){
    resetTime();
    time = t;

    //do soft resets
    tracks.runAfter(time, [](const TimeFormat myTime, const AnimationList::TimeItem& item) {
            item.value->reset(true);
    });

    compositors.runAfter(time, [](const TimeFormat myTime, const CompositList::TimeItem& item) { 
            item.value->reset(true);
    });

    actions.runAfter(time, [](const TimeFormat myTime, const ActionList::TimeItem& item) {             
            item.value->reset(true);
    });
}

void Timeline::addTrack(AnimationList::PItemType track, TimeFormat startTime, TimeFormat playbackDuration, LEDState::LayerTypes layer){
    tracks.addItem(track, startTime, playbackDuration, layer);
}

void Timeline::addCompositor(CompositList::PItemType track, TimeFormat startTime, TimeFormat playbackDuration, LEDState::LayerTypes layer){
    compositors.addItem(track, startTime, playbackDuration, layer);
}

void Timeline::addAction(ActionList::PItemType track, TimeFormat timeStamp, LEDState::LayerTypes layer){
    actions.addItem(track, timeStamp, 0, layer);
}
