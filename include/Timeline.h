#ifndef __TIMELINE_H__
#define __TIMELINE_H__

#include "LEDViews.h"

#define FPS 60

class Animation;
struct TimeTrack {
    Animation* anim;
    double startTime;
    double duration;
    LEDState::LayerTypes layer;
};

class Timeline{
    public:
        Timeline();

        void tick();
        
        inline void begin() {
            reset();
            resume();            
        }

        inline void pause() { active = false; }

        inline void resume() { 
            lastTick = millis();
            active = true; 
        }
    protected:
        void reset();
        
    private:
        unsigned long lastTick;
        unsigned long accDelta;
        double time;
        bool active;
};

#endif