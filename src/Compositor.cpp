#include "Animation.h"
#include "Compositor.h"

Compositor::Compositor(double duration, CompositRetimeFunction tmFkt) : duration(duration), timeFunction(tmFkt) {
     
}

void DefaultCompositor::compositIntern(double time, uint8_t layerMask, LEDState::LayerTypes backlayer, LEDState::LayerTypes frontlayer){
    for (int c=0; c<view->width; c++){
        for (int r=0; r<view->height; r++){
            
            CRGB cl = view->pixelFromLayer(c, r, backlayer);
            for (uint8_t l=LEDState::LayerTypes::CompositBottomMostLayer; l>=LEDState::LayerTypes::CompositTopMostLayer; l--) {
                if (hasBit(layerMask, l)){
                    const float a = view->alphaFromLayer(c, r, (LEDState::LayerTypes)l);
                    const CRGB c = view->pixelFromLayer(c, r, (LEDState::LayerTypes)l);
                    cl = blendFunction(cl, c, a);
                }
            }
            view->alphaFromLayer(c, r, frontlayer) = 1.0f;
            view->pixelFromLayer(c, r, frontlayer) = cl;
        }
    }
}