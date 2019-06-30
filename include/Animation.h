#ifndef __Animation__h__
#define __Animation__h__
#include "LEDViews.h"


/*
    Animation: calculates colors based on time (in seconds)
        - does not composit
        - writes to one of the available layers
        - can have alpha
        - must be able to play endless 
            - use TimeConverter to repeat or flipflop automatically
            - Default TimeConverter is identity
            - If animation has time limit (duration>0), repeat and flipFlop are available
    Timeline: contains a set of animation and composit tracks based on an animation time
        - Tracks contain Animations that start at given times and play for a given duration
        - Animation Tracks write to defined Layers in given order (Background, Bottom, Middle, Top)
            - Composit Tracks can create the FinalComposit Layer from the animation data in lower layers
            - Default Composit is copy
            - Composit can be used to build Transitions
            - Composit = specialization of Animation class?
    Action: Part of the TimeLine, can influence the animation time and speed
        - stop (will halt the animation)
        - speed (will change the time scale)
        - wait (will wait for an event)  
    Trsansition: transitions from one timeline to another (smoothly swap animation)
        - Has a type
        - Timeline has to composit to a given layer (first in transition to TransitionBackBuffer)
        - Maybe adapt the final compositor for the top timeline
        - Otherwise write second timeline to FinalComposit and dp another composit run on top

 */

typedef float TimeFormat;
typedef std::function<TimeFormat (const TimeFormat, const class Animation*)> AnimationRetimeFunction;

template <class T>
inline TimeFormat timeIdentity(const TimeFormat inTime, const T* a){
    return inTime;
}

class Animation {
    public:        
        Animation(TimeFormat duration=0, AnimationRetimeFunction tmFkt=timeIdentity<Animation>);

        /*
        soft = false  => timeline needs to be restarted
        soft = true   => timeline moved to a previous timestamp => do not reset state of the object but make sure it can be enabled again
         */
        inline void reset(bool soft=false) const {}
        inline void render(TimeFormat time, LEDState::LayerTypes layer = LEDState::LayerTypes::FinalComposit) {
            renderIntern(timeFunction(time, this), layer);
        }

        const TimeFormat duration;
        const AnimationRetimeFunction timeFunction;
    protected:
        virtual void renderIntern(TimeFormat time, LEDState::LayerTypes layer) = 0;
    private:
        
};

class TestAnimation : public Animation {
    public:        
        TestAnimation(std::string n, TimeFormat duration=0, AnimationRetimeFunction tmFkt=timeIdentity<Animation>) : Animation(duration, tmFkt), name(n) {

        }
    protected:
        virtual void renderIntern(TimeFormat time, LEDState::LayerTypes layer){
            Serial.printf("    - Render to layer %s: %d @ %f\n", name.c_str(), layer, time);
        }

    private:
        std::string name;
};

class Animation2D : public Animation {
    public:        
        Animation2D(class LEDView2* view, TimeFormat duration=0, AnimationRetimeFunction tmFkt=timeIdentity<Animation>) : Animation(duration, tmFkt), view(view){}
    protected:
        class LEDView2 * const view;
};

class SolidColorAnimation : public Animation2D {
    public:        
        SolidColorAnimation(class LEDView2* view, CRGB cl, TimeFormat duration=0, AnimationRetimeFunction tmFkt=timeIdentity<Animation>) : Animation2D(view, duration, tmFkt), cl(cl) {}
    protected:
        virtual void renderIntern(TimeFormat time, LEDState::LayerTypes layer);
    private:
        const CRGB cl;
};

class HorizontalWipeAnimation : public Animation2D {
    public:        
        HorizontalWipeAnimation(class LEDView2* view, CRGB cl, TimeFormat duration=0, AnimationRetimeFunction tmFkt=timeIdentity<Animation>) : Animation2D(view, duration, tmFkt), cl(cl) {}
    protected:
        virtual void renderIntern(TimeFormat time, LEDState::LayerTypes layer);
    private:
        const CRGB cl;
};

template <class T>
inline TimeFormat timeRepeat(const TimeFormat inTime, const T* a){
    TimeFormat t = inTime;
    while (t>=a->duration) t -= a->duration;
    return t;
}

template <class T>
inline TimeFormat timeBounce(const TimeFormat inTime, const T* a){
    TimeFormat t = inTime;
    while (t>=2*a->duration) t -= 2*a->duration;
    if (t>=a->duration) {
        t = (2*a->duration-1) - t;
    }
    return t;
}

#endif