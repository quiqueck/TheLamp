#ifndef __COMPOSITOR_H__
#define __COMPOSITOR_H__
#include "LEDViews.h"
#include "Animation.h"

typedef std::function<CRGB (const CRGB&, const CRGB&, float)> BlendFunction;
inline CRGB keepBlend(const CRGB& c0, const CRGB& c1, float a){
    return c0;
}

inline CRGB overBlend(const CRGB& c0, const CRGB& c1, float a){
    return c1;
}

inline CRGB alphaBlend(const CRGB& c0, const CRGB& c1, float a){
    const float aa = 1.0f-a;
    return CRGB(c1.r * a + c0.r * aa, c1.g * a + c0.g * aa, c1.b * a + c0.b * aa);
}

typedef std::function<double (const double, const class Compositor*)> CompositRetimeFunction;

class Compositor{
    public:        
        Compositor(double duration=0, CompositRetimeFunction tmFkt=timeIdentity<Compositor>);

        inline void reset(bool soft=false) const {}
        inline void composit(double time, uint8_t layerMask, LEDState::LayerTypes backlayer = LEDState::LayerTypes::SolidBackground, LEDState::LayerTypes frontlayer = LEDState::LayerTypes::FinalComposit) {
            compositIntern(timeFunction(time, this), layerMask, backlayer, frontlayer);
        }

        const double duration;
        const CompositRetimeFunction timeFunction;
    protected:
        virtual void compositIntern(double time, uint8_t layerMask, LEDState::LayerTypes backlayer, LEDState::LayerTypes frontlayer) = 0;
    private:
        
};

class TestCompositor : public Compositor{
    public:        
        TestCompositor(std::string n, double duration=0, CompositRetimeFunction tmFkt=timeIdentity<Compositor>) : Compositor(duration, tmFkt), name(n) {}      
    protected:
        virtual void compositIntern(double time, uint8_t layerMask, LEDState::LayerTypes backlayer, LEDState::LayerTypes frontlayer) {
            Serial.printf("    - Composit %s @ %f: %d -> %d\n",  name.c_str(), time, backlayer, frontlayer);
            for (uint8_t l=LEDState::LayerTypes::CompositBottomMostLayer; l>=LEDState::LayerTypes::CompositTopMostLayer; l--) {
                if (hasBit(layerMask, l)){
                    Serial.printf("        + %d\n", l);
                }
            }
            
        }

    private:
        std::string name;
};

class DefaultCompositor : public Compositor{
    public:        
        DefaultCompositor(class LEDView2* view, double duration=0, BlendFunction blendFunction=alphaBlend, CompositRetimeFunction tmFkt=timeIdentity<Compositor>) : Compositor(duration, tmFkt), view(view), blendFunction(blendFunction) {}

        class LEDView2 * const view;
        const BlendFunction blendFunction;        
    protected:
        virtual void compositIntern(double time, uint8_t layerMask, LEDState::LayerTypes backlayer, LEDState::LayerTypes frontlayer);
};

#endif