#ifndef __TIMELINE_H__
#define __TIMELINE_H__

#define FPS 60

class Timeline{
    public:
        Timeline();

        void tick();
        void reset();
    private:
        unsigned long lastTick;
        unsigned long accDelta;
        double time;
};

#endif