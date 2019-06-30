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
    const float pos = (float)(view->width * p);
    float posI = 0.0f;
    const float posF = modff(pos, &posI);
    
    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){
            if (c < posI) {
                view->pixelFromLayer(c, r, layer) = cl;
                view->alphaFromLayer(c, r, layer) = 1.0f;
            } else if ( c == posI ) {
                view->pixelFromLayer(c, r, layer) = cl;
                view->alphaFromLayer(c, r, layer) = posF;
            } else {
                view->alphaFromLayer(c, r, layer) = 0.0f;
            }            
        }
    }
}