#ifndef __VirtualGrid__h__
#define __VirtualGrid__h__
#include "LEDGrid.h"
#include <vector>

class VirtualGrid : public LEDView2, public LEDView1 {
    public:
        VirtualGrid(int width, int height);
        ~VirtualGrid();
        virtual CRGB &operator()(int x, int y) { return pixel(x, y); }
        virtual CRGB& pixel(int x, int y);
        virtual CRGB& pixelFromLayer(int x, int y, LayerTypes layer);
        virtual float& alphaFromLayer(int x, int y, LayerTypes layer);  
        
        virtual CRGB& operator[](int i) { return pixel(i); }
        virtual CRGB& pixel(int i) { return pixel(i%width, i/width);}
        virtual CRGB& pixelFromLayer(int i, LayerTypes layer) { return pixelFromLayer(i%width, i/width, layer);}
        virtual float& alphaFromLayer(int i, LayerTypes layer) { return alphaFromLayer(i%width, i/width, layer);} 

        void addPanel(LEDView2* p, int c, int r) { 
            Descriptor* d = new Descriptor();
            d->panel = p;
            d->r = r;
            d->c = c;
            panels.push_back(d); 
        }

    protected:
        struct Descriptor {
            LEDView2* panel;
            int r, c;
        };

        inline bool inRange(VirtualGrid::Descriptor* d, int x, int y) const {
            return  (((y-(d->r + d->panel->height - 1)))*(y-d->r) <= 0) && (((x-(d->c + d->panel->width - 1)))*(x-d->c) <= 0); 
        }
        std::vector<Descriptor*> panels;

    private:
        CRGB dummy;
        float dummyAlpha;
};
#endif