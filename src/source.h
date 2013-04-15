#pragma once

#include "ofMain.h"
#include "capture.h"

#ifndef SOURCE_H
#define SOURCE_H

enum resourceType
{
    SRC_SOLIDCOLOR   = 0,
    SRC_IMAGE 	     = 1,
    SRC_VIDEO	     = 2
};

class Source
{
public:

    // resource handling
    resourceType mtype;
    string path;
    ofImage         srcImg;
    ofVideoGrabber  srcVideo;
    bool bNewFrame = false;
    int width, height;

    // filters
    Capture* 		fltrFlashLight;

    Source(string src, resourceType type);
    virtual ~Source();
    void setup();
    void update();

    void getFrame(ofPixels* dst);

protected:
private:
    void (Source::*setup_func)();
    void (Source::*update_func)();
    void (Source::*getFrame_func)(ofPixels* dst);

    /*void setup_func();
    void update_func();
    void getFrame_func(ofPixels* dst);*/

    void setup_image();
    void update_image();
    void getFrame_image(ofPixels* dst);
};


#endif // SOURCE_H
