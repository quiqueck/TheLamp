#include "Animation.h"

Animation::Animation(double duration, AnimationRetimeFunction tmFkt) : duration(duration), timeFunction(tmFkt) {
     
}

void SolidColorAnimation::renderIntern(double time, LEDState::LayerTypes layer){
    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){
            view->pixelFromLayer(c, r, layer) = cl;
            view->alphaFromLayer(c, r, layer) = 1.0f;
        }
    }
}

void HorizontalFillAnimation::renderIntern(double time, LEDState::LayerTypes layer){
    const double p = time/duration;
    
    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){
            view->pixelFromLayer(c, r, layer) = cl;
            view->alphaFromLayer(c, r, layer) = 1.0f;
        }
    }
}