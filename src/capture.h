#pragma once

#include "ofxOpenCv.h"

#ifndef CAPTURE_H
#define CAPTURE_H


class Capture
{
    public:
        unsigned int id; // the device id
        string name; // the device name string
        ofVideoGrabber& 		vidGrabber;
        bool bNewFrame = false;
        bool bLearnBakground = true;

        // ofxOpenCv image types for storing intermediate representations (useful for monitoring, etc.)
        ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;
        ofxCvGrayscaleImage 	grayBg;

        // this is the final output from capture processing
        ofxCvGrayscaleImage 	grayDiff;

        Capture(int deviceID, string deviceName, ofVideoGrabber& g);
        virtual ~Capture();
        void setup();
        void update();
        void learnBackground();

        void getAlphaMask(ofPixels* dst);

    protected:
    private:
};


#endif // CAPTURE_H
