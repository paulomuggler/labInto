#include "capture.h"

Capture::Capture(ofVideoGrabber& g):vidGrabber(g){}

Capture::~Capture()
{
    //dtor
}

void Capture::setup(){

    vidGrabber.setVerbose(true);
    vidGrabber.setPixelFormat(OF_PIXELS_RGB);
    vidGrabber.setDesiredFrameRate(50);
    vidGrabber.initGrabber(320,240);

}

void Capture::update(){

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame){

        if (bLearnBakground == true)
        {
            grayBg = grayImage;
            bLearnBakground = false;
        }
    }

}

void Capture::learnBackground(){
    bLearnBakground = true;
}
