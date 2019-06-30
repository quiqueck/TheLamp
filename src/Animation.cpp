#include "Animation.h"

Animation::Animation(TimeFormat duration, AnimationRetimeFunction tmFkt) : duration(duration), timeFunction(tmFkt) {
     
}

void SolidColorAnimation::renderIntern(TimeFormat time, LEDState::LayerTypes layer){
    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){
            view->pixelFromLayer(c, r, layer) = cl;
            view->alphaFromLayer(c, r, layer) = 1.0f;
        }
    }
}

void HorizontalWipeAnimation::renderIntern(TimeFormat time, LEDState::LayerTypes layer){
    const TimeFormat p = time/duration;
    const float pos = (float)((view->width + spread) * p);
    

    //Serial.printf("%f %f %f: %.0f, %f\n", time, duration, pos, posI, posF);
    
    for (int c=0; c<view->width; c++){
        const float dist = (c + spread) - pos;
        for (int r=0; r<view->height; r++){
            if (dist < 0) {
                view->pixelFromLayer(c, r, layer) = cl;
                view->alphaFromLayer(c, r, layer) = 1.0f;
            } else if ( dist > spread ) {
                view->alphaFromLayer(c, r, layer) = 0.0f;                
            } else {
                view->pixelFromLayer(c, r, layer) = cl;
                view->alphaFromLayer(c, r, layer) = 1.0f - (dist / spread);
            }            
        }
    }
}

void VerticalWipeAnimation::renderIntern(TimeFormat time, LEDState::LayerTypes layer){
    const TimeFormat p = time/duration;
    const float pos = (float)((view->height + spread) * p);
    

    //Serial.printf("%f %f %f: %.0f, %f\n", time, duration, pos, posI, posF);
    
    for (int r=0; r<view->height; r++){
        const float dist = (r + spread) - pos;
        for (int c=0; c<view->width; c++){
            if (dist < 0) {
                view->pixelFromLayer(c, r, layer) = cl;
                view->alphaFromLayer(c, r, layer) = 1.0f;
            } else if ( dist > spread ) {
                view->alphaFromLayer(c, r, layer) = 0.0f;                
            } else {
                view->pixelFromLayer(c, r, layer) = cl;
                view->alphaFromLayer(c, r, layer) = 1.0f - (dist / spread);
            }            
        }
    }
}