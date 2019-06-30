#ifndef __LEDViews__h__
#define __LEDViews__h__

#define SPI_DATA 13   //ESP 32, MOSI
#define SPI_CLOCK 14   
#define SPI_CLK_PIN SPI_CLOCK
#define SPI_DATA_PIN SPI_DATA

#include <FastLED.h>
#include <memory>
class Animation;

class LEDState {
    public: 
        enum LayerTypes { FinalComposit=0x00, TopOverlay=0x01, MiddleOverlay=0x02, BottomOverlay=0x03, SolidBackground=0x04, TransitionBackBuffer=0x05, LayerTypeCount=0x06, CompositBottomMostLayer=0x03, CompositTopMostLayer=0x01 };
        
        inline void setLastAnimation(Animation* anim, int frame) {
            lastAnimation = anim;
            lastAnimationFrame = frame;
        };

        inline bool changed(Animation* anim, int frame) const {
            return frame!=lastAnimationFrame || anim!=lastAnimation;
        }
    protected:
        LEDState() : lastAnimation(nullptr), lastAnimationFrame(-1) {
            
        }
        Animation* lastAnimation;
        int lastAnimationFrame;
};


class LEDView1  : public LEDState {
    public:
        virtual CRGB& operator[](int i) = 0;
        virtual CRGB& pixel(int i) = 0;
        virtual CRGB& pixelFromLayer(int i, LayerTypes layer) = 0;
        virtual float& alphaFromLayer(int i, LayerTypes layer) = 0;  
        
        const uint16_t count;
    protected:
        LEDView1(uint16_t pixelCount) : LEDState(), count(pixelCount) {}
};

class LEDView2 : public LEDState{
    public:
        virtual CRGB& operator()(int x, int y) = 0;
        virtual CRGB& pixel(int x, int y) = 0;
        virtual CRGB& pixelFromLayer(int x, int y, LayerTypes layer) = 0; 
        virtual float& alphaFromLayer(int x, int y, LayerTypes layer) = 0; 

        void fill(CRGB cl) {
            for (uint16_t c=0; c<width; c++){
                for (uint16_t r=0; r<height; r++){
                    pixel(c, r) = cl;
                }
            }
        }

        const uint16_t pixelCount;
        const uint16_t width;
        const uint16_t height;
    protected:
        LEDView2(uint16_t pixelWidth, uint16_t pixelHeight) : LEDState(), pixelCount(pixelWidth*pixelHeight), width(pixelWidth), height(pixelHeight) {}
};

inline uint8_t toBit(const LEDState::LayerTypes layer) {
    return 1 << layer;
}

inline void addBit(uint8_t& mask, const LEDState::LayerTypes layer) {
    mask = mask | toBit(layer);
}

inline bool hasBit(const uint8_t mask, const LEDState::LayerTypes layer) {
    return (mask & toBit(layer)) != 0;
}

inline bool hasBit(const uint8_t mask, const uint8_t layer) {
    return (mask & toBit((LEDState::LayerTypes)layer)) != 0;
}


#endif