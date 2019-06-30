#ifndef __TRANSITION_H__
#define __TRANSITION_H__
#include "Timeline.h"

class Transition : public Tickable{
    public:
        Transition(double duration, std::shared_ptr<Timeline> src, std::shared_ptr<Timeline> tgt);
        inline bool finished() const { return time>=duration; }
        void runAt(double time);
    protected:
        virtual void reset();
        virtual void runAndComposit(double alpha) = 0;
        const double duration;
        const std::shared_ptr<Timeline> src;
        const std::shared_ptr<Timeline> tgt;
};

class AlphaTransition : public Transition{
    public:
        AlphaTransition(class LEDView2* view, double duration, std::shared_ptr<Timeline> src, std::shared_ptr<Timeline> tgt) 
            : Transition(duration, src, tgt), view(view) {}
        
    protected:
        class LEDView2 * const view;
        virtual void runAndComposit(double alpha);        
};

#endif