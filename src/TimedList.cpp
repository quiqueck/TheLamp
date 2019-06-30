#include "TimedList.h"
#include "Animation.h"
#include "Compositor.h"
#include "Action.h"

template<class ItemType> 
void TimedList<ItemType>::addItem(PItemType item, double startTime, double duration, LEDState::LayerTypes layer){
    TimeItem t;
    t.startTime = startTime;
    t.endTime = (duration<=0) ? std::numeric_limits<double>::max() : startTime + duration;
    t.layer = layer;
    t.value = item;

    items.push_back(t);
}

template<class ItemType> 
void TimedList<ItemType>::each(double time, RunFunction fkt){
    for (const TimeItem &i : items) {
        fkt(time - i.startTime, i);
    }
}

template<class ItemType> 
void TimedList<ItemType>::runAt(double time, RunFunction fkt){
    for (const TimeItem &i : items) {
        if (i.startTime <= time && i.endTime > time) {            
            fkt(time - i.startTime, i);
        }
    }
}

template<class ItemType> 
void TimedList<ItemType>::runAfter(double time, RunFunction fkt){
    for (const TimeItem &i : items) {
        if (i.startTime >= time) {            
            fkt(time - i.startTime, i);            
        }
    }
}

template<class ItemType> 
void TimedList<ItemType>::runBefore(double time, RunFunction fkt){
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