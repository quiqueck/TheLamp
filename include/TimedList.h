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
        virtual TimeFormat startTime() const = 0;
        virtual TimeFormat endTime() const = 0;
};

template<class ItemType> 
class TimedList {
    public:
        typedef std::shared_ptr<ItemType> PItemType;
        struct TimeItem {
            PItemType value;
            TimeFormat startTime;
            TimeFormat endTime;
            LEDState::LayerTypes layer;
        };
        typedef std::function<void (const TimeFormat, const TimeItem&)> RunFunction;

    public:
        void addItem(PItemType item, TimeFormat startTime, TimeFormat duration, LEDState::LayerTypes layer);
        void each(TimeFormat time, RunFunction fkt);
        void runAt(TimeFormat time, RunFunction fkt);
        void runAfter(TimeFormat time, RunFunction fkt);
        void runBefore(TimeFormat time, RunFunction fkt);
        void reset();
    protected:

    private:
    std::vector<TimeItem> items;
};

typedef TimedList<Animation> AnimationList;
typedef TimedList<Compositor> CompositList;
typedef TimedList<Action> ActionList;
#endif