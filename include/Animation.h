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

 */

typedef std::function<double (const double, const Animation*)> RetimeFunction;
inline double timeIdentity(const double inTime, const Animation* a){
    return inTime;
}

class Animation {
    public:        
        Animation(double duration=0, RetimeFunction tmFkt=timeIdentity);

        inline void render(double time, LEDState::LayerTypes layer = LEDState::LayerTypes::FinalComposit) {
            renderIntern(timeFunction(time, this), layer);
        }

        const double duration;
        const RetimeFunction timeFunction;
    protected:
        virtual void renderIntern(double time, LEDState::LayerTypes layer) = 0;
    private:
        
};



inline double timeRepeat(const double inTime, const Animation* a){
    double t = inTime;
    while (t>a->duration) t -= a->duration;
    return t;
}

#endif