#ifndef __Animation__h__
#define __Animation__h__
#include "LEDViews.h"
#include <memory>
#define FPS 60

/*
    Animation: calculates colors based on time (in seconds)
        - does not composit
        - writes to one of the available layers
        - can have alpha
        - must be able to play endless 
            - use TimeConverter to repeat or flipflop automatically
            - Default TimeConverter is identity
            - If animation has time limit (duration>0), repeat and flipFlop are available
    Timeline: contains a set of animation and composit tracks based on an animation time
        - Tracks contain Animations that start at given times and play for a given duration
        - Animation Tracks write to defined Layers in given order (Background, Bottom, Middle, Top)
            - Composit Tracks can create the FinalComposit Layer from the animation data in lower layers
            - Default Composit is copy
            - Composit can be used to build Transitions
            - Composit = specialization of Animation class?
    Action: Part of the TimeLine, can influence the animation time and speed
        - stop (will halt the animation)
        - speed (will change the time scale)
        - wait (will wait for an event)        

 */

inline CRGB keepBlend(const CRGB& c0, const CRGB& c1, float a){
    return c0;
}

inline CRGB overBlend(const CRGB& c0, const CRGB& c1, float a){
    return c1;
}

inline CRGB alphaBlend(const CRGB& c0, const CRGB& c1, float a){
    const float aa = 1.0f-a;
    return CRGB(c1.r * a + c0.r * aa, c1.g * a + c0.g * aa, c1.b * a + c0.b * aa);
}

class Animation {
    public:
        enum RepeatStyle { none, repeat, pingpong };
        typedef std::function<CRGB (const CRGB&, const CRGB&, float)> BlendFunction;
        
        Animation(std::shared_ptr<Animation> next, int frameCount) : frameCount(frameCount), next(next) {   
            frame = -1;
            currentRepeatCount = 1;
            countUp = true;
            repeatCount = 1;
            repeatStyle = repeat;
            
        }
        inline void setNext(std::shared_ptr<Animation> n) { next = n; }
        inline std::shared_ptr<Animation> getNext() const { return next; }
        inline void setRepeatStyle(RepeatStyle st) { repeatStyle = st; }
        inline void setRepeatCount(int ct) { repeatCount = ct; }

        virtual void reset() { frame = -1; currentRepeatCount = 1; countUp = true;};
        virtual bool showNext(BlendFunction blendFkt = nullptr, float blendAlpha = 1.0f) {
            if (blendFkt==nullptr) blendFkt = overBlend;

    
            if (countUp) frame = max(0, min(frameCount, frame+1));
            else frame = max(0, min(frameCount, frame-1));

            showFrame(max(0, min(frameCount-1, frame)), blendFkt, blendAlpha);

            //last repeat when trending upward
            if (repeatStyle==none || (countUp && currentRepeatCount == repeatCount)) {
                const auto tStart = frameCount - transitionFrames();
                if (next != nullptr && frame >= tStart) {
                    if (next.get() != this && frame == tStart) next->reset();  
                    next->showNext(transitionFunction(), ((frame-tStart) / (float)transitionFrames()));
                } else if (next != nullptr && frame == frameCount-1) {
                    next->reset();  
                    return true;              
                } else if (next == nullptr) {
                    frame = -1;
                    currentRepeatCount++;
                    return false;
                }
            } else if (countUp){
                if (frame == frameCount-1){
                    if (repeatStyle == repeat){
                        frame = -1;
                        currentRepeatCount++;
                        return false;
                    } else if (repeatStyle == pingpong) {
                        countUp = false;
                        return false;
                    }
                }
            } else {
                if (frame==0) {
                    countUp = true;
                    currentRepeatCount++;
                    return false;
                }
            }

            return frame>=frameCount;
        };

        virtual int transitionFrames() { return 120; }
        virtual BlendFunction transitionFunction() { return alphaBlend; }
    protected:
        const int frameCount;
        int frame;

        int repeatCount;
        RepeatStyle repeatStyle;

        bool countUp;
        int currentRepeatCount;
        virtual void showFrame(int f, BlendFunction blendFkt, float blendAlpha) = 0;
        std::shared_ptr<Animation> next;
    private:
};

#endif