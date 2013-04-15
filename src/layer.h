#pragma once

#include "ofMain.h"
#include "capture.h"
#include "source.h"

#ifndef LAYER_H
#define LAYER_H

class Layer
{
public:

    Source* source;
    ofPixels          mPixMask;
    ofPixels          mPixOut;
    ofTexture         mTex;
    int width, height;

    // filters
    Capture*		fltrFlashLight;

    Layer(Source* src, int w, int h);
    virtual ~Layer();
    void setup();
    void update();
    void draw();

    void draw(int x1, int y1, int x2, int y2);

    void getFrame(ofPixels* dst);

protected:
private:
};


#endif // LAYER_H

