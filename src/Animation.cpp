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

void HorizontalFillAnimation::renderIntern(TimeFormat time, LEDState::LayerTypes layer){
    const TimeFormat p = time/duration;

    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){
            view->pixelFromLayer(c, r, layer) = cl;
            view->alphaFromLayer(c, r, layer) = 1.0f;
        }
    }
}