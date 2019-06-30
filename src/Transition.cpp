#include "Transition.h"

Transition::Transition(double duration, std::shared_ptr<Timeline> src, std::shared_ptr<Timeline> tgt) : Tickable(), duration(duration), src(src), tgt(tgt) {

}

void Transition::runAt(double time){
    src->tick(false, false);
    tgt->tick(false, false);

    runAndComposit(min(1.0, max(0.0, time/duration)));
}

void AlphaTransition::runAndComposit(double alpha){
    const uint8_t smask = src->runWithTransition();
    src->compositWithTransition(smask, LEDState::LayerTypes::SolidBackground, LEDState::LayerTypes::TransitionBackBuffer);

    const uint8_t tmask = tgt->runWithTransition();
    tgt->compositWithTransition(tmask, LEDState::LayerTypes::SolidBackground, LEDState::LayerTypes::FinalComposit);

    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){            
            CRGB cl = view->pixelFromLayer(c, r, LEDState::LayerTypes::TransitionBackBuffer);
            const CRGB c = view->pixelFromLayer(c, r, LEDState::LayerTypes::FinalComposit);
            
            view->pixelFromLayer(c, r, LEDState::LayerTypes::FinalComposit) = alphaBlend(cl, c, alpha);
        }
    }
}

void Transition::reset(){
    resetTime();

    //make sure the target timeline is at beginning
    tgt->begin();
}