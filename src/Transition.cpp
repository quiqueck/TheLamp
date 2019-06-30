#include "Transition.h"

Transition::Transition(TimeFormat duration, std::shared_ptr<Timeline> src, std::shared_ptr<Timeline> tgt) : Tickable(), duration(duration), src(src), tgt(tgt) {

}

void Transition::runAt(TimeFormat time){
    src->tick(false, false);
    tgt->tick(false, false);
    const TimeFormat x = time/duration;
    

    runAndComposit(min(1.0f, max(0.0f, 3*x*x - 2*x*x*x)));//smooth-step
    //runAndComposit(min(1.0f, max(0.0f, x)));
}

void BlendTransition::runAndComposit(TimeFormat alpha){
    const uint8_t smask = src->runWithTransition();
    src->compositWithTransition(
        smask, 
        LEDState::LayerTypes::SolidBackground, 
        LEDState::LayerTypes::TransitionBackBuffer);

    const uint8_t tmask = tgt->runWithTransition();
    tgt->compositWithTransition(
        tmask, 
        LEDState::LayerTypes::SolidBackground, 
        LEDState::LayerTypes::FinalComposit);

    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){            
            const CRGB c0 = view->pixelFromLayer(c, r, LEDState::LayerTypes::TransitionBackBuffer);
            const CRGB c1 = view->pixelFromLayer(c, r, LEDState::LayerTypes::FinalComposit);
            
            view->pixelFromLayer(c, r, LEDState::LayerTypes::FinalComposit) = alphaBlendWithGamma(c0, c1, alpha);
        }
    }
}

void Transition::reset(){
    resetTime();

    //make sure the target timeline is at beginning
    tgt->begin();
}