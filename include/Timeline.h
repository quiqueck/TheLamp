#ifndef __TIMELINE_H__
#define __TIMELINE_H__

#include "LEDViews.h"
#include "TimedList.h"
#include "Animation.h"

#define FPS 60

class Animation;

class Timeline{
    public:
        Timeline();

        inline void addTrack(AnimationList::PItemType track, double startTime, LEDState::LayerTypes layer){
            addTrack(track, startTime, track->duration, layer);
        }

        inline void addCompositor(CompositList::PItemType track, double startTime, LEDState::LayerTypes layer){
            addCompositor(track, startTime, track->duration, layer);
        }
        /**
         * playbackDuration: <=0 results in endless palayback
         */
        void addTrack(AnimationList::PItemType track, double startTime, double playbackDuration, LEDState::LayerTypes layer);
        void addCompositor(CompositList::PItemType track, double startTime, double playbackDuration, LEDState::LayerTypes layer);

        void tick();
        void runAt(double time);
        
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

        AnimationList tracks;
        CompositList compositors;
};

#endif