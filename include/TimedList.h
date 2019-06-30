#ifndef __TIMED_LIST_H__
#define __TIMED_LIST_H__

#include "LEDViews.h"
#include "Animation.h"
#include "Compositor.h"
#include "Action.h"
#include <vector>
#include <memory>

class TimeItem {
    public:
        virtual double startTime() const = 0;
        virtual double endTime() const = 0;
};

template<class ItemType> 
class TimedList {
    public:
        typedef std::shared_ptr<ItemType> PItemType;
        struct TimeItem {
            PItemType value;
            double startTime;
            double endTime;
            LEDState::LayerTypes layer;
        };
        typedef std::function<void (const double, const TimeItem&)> RunFunction;

    public:
        void addItem(PItemType item, double startTime, double duration, LEDState::LayerTypes layer);
        void each(double time, RunFunction fkt);
        void runAt(double time, RunFunction fkt);
        void runAfter(double time, RunFunction fkt);
        void runBefore(double time, RunFunction fkt);
        void reset();
    protected:

    private:
    std::vector<TimeItem> items;
};

typedef TimedList<Animation> AnimationList;
typedef TimedList<Compositor> CompositList;
typedef TimedList<Action> ActionList;
#endif