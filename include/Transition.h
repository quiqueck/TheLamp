#ifndef __TRANSITION_H__
#define __TRANSITION_H__
#include "Timeline.h"

class Transition : public Tickable{
    public:
        Transition(TimeFormat duration, std::shared_ptr<Timeline> src, std::shared_ptr<Timeline> tgt);
        inline bool finished() const { return time>=duration; }
        void runAt(TimeFormat time);
    protected:
        virtual void reset();
        virtual void runAndComposit(TimeFormat alpha) = 0;
        const TimeFormat duration;
        const std::shared_ptr<Timeline> src;
        const std::shared_ptr<Timeline> tgt;
};

class BlendTransition : public Transition{
    public:
        BlendTransition(class LEDView2* view, TimeFormat duration, std::shared_ptr<Timeline> src, std::shared_ptr<Timeline> tgt) 
            : Transition(duration, src, tgt), view(view) {}
        
    protected:
        class LEDView2 * const view;
        virtual void runAndComposit(TimeFormat alpha);        
};

#endif