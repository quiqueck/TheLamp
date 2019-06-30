#ifndef __TIMELINE_H__
#define __TIMELINE_H__

#include "LEDViews.h"
#include "TimedList.h"
#include "Animation.h"

#define FPS 2

class Animation;

class Tickable {
    friend class Transition;
    public:
        Tickable();
                
        void tick(bool earlyOut=true, bool callRun=true);
        virtual void runAt(double time) = 0;

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
        virtual void reset() = 0;
        void resetTime();

        unsigned long lastTick;
        unsigned long accDelta;
        double time; 
        bool active;       
};

class Timeline : public Tickable{    
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

        void addAction(ActionList::PItemType track, double timeStamp, LEDState::LayerTypes layer=LEDState::LayerTypes::FinalComposit);
        
        void runAt(double time);        
        void goToTime(double time);
        
        uint8_t runWithTransition();
        void compositWithTransition(uint8_t layerMask, LEDState::LayerTypes Background, LEDState::LayerTypes Final);
    protected:
        
        virtual void reset();

    private:

        AnimationList tracks;
        CompositList compositors;
        ActionList actions;
};

#endif