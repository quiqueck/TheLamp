#include "VirtualGrid.h"
#include "Animation.h"


VirtualGrid::VirtualGrid(int width, int height) : LEDView2(width, height), LEDView1(width*height){
    dummy = CRGB::White;
    dummyAlpha = 1.0f;
}

 VirtualGrid::~VirtualGrid(){
    for (Descriptor* d : panels ) {
        delete(d);   
    } 
 }

float& VirtualGrid::alphaFromLayer(int c, int r, LayerTypes layer) {
    for (Descriptor* d : panels ) {
        if (inRange(d, c, r)){
            return d->panel->alphaFromLayer(c - d->c, r - d->r, layer);
        }
    }

    return dummyAlpha;
}


CRGB& VirtualGrid::pixelFromLayer(int c, int r, LayerTypes layer){
    for (Descriptor* d : panels ) {
        if (inRange(d, c, r)){
            return d->panel->pixelFromLayer(c - d->c, r - d->r, layer);
        }
    }

    return dummy;
}

CRGB& VirtualGrid::pixel(int c, int r) {
    for (Descriptor* d : panels ) {
        if (inRange(d, c, r)){
            return d->panel->pixel(c - d->c, r - d->r);
        }
    }

    return dummy;
}