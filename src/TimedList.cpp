#include "TimedList.h"
#include "Animation.h"
#include "Compositor.h"
#include "Action.h"

template<class ItemType> 
void TimedList<ItemType>::addItem(PItemType item, TimeFormat startTime, TimeFormat duration, LEDState::LayerTypes layer){
    TimeItem t;
    t.startTime = startTime;
    t.endTime = (duration<=0) ? std::numeric_limits<TimeFormat>::max() : startTime + duration;
    t.layer = layer;
    t.value = item;

    items.push_back(t);
}

template<class ItemType> 
void TimedList<ItemType>::each(TimeFormat time, RunFunction fkt){
    for (const TimeItem &i : items) {
        fkt(time - i.startTime, i);
    }
}

template<class ItemType> 
void TimedList<ItemType>::runAt(TimeFormat time, RunFunction fkt){
    for (const TimeItem &i : items) {
        if (i.startTime <= time && i.endTime > time) {            
            fkt(time - i.startTime, i);
        }
    }
}

template<class ItemType> 
void TimedList<ItemType>::runAfter(TimeFormat time, RunFunction fkt){
    for (const TimeItem &i : items) {
        if (i.startTime >= time) {            
            fkt(time - i.startTime, i);            
        }
    }
}

template<class ItemType> 
void TimedList<ItemType>::runBefore(TimeFormat time, RunFunction fkt){
    for (const TimeItem &i : items) {
        if (i.startTime <= time) {            
            fkt(time - i.startTime, i);            
        }
    }
}

template<class ItemType> 
void TimedList<ItemType>::reset(){
    for (const TimeItem &i : items) {
        i.value->reset();
    }
}

template class TimedList<Animation>;
template class TimedList<Compositor>;
template class TimedList<Action>;