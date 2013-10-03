#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxSyphon.h"

#include "capture.h"


class testApp : public ofBaseApp
{

public:

    // application lifecycle methods
    void setup();
    void update();
    void draw();

    // UI methods
    void keyPressed(int key);

    // video capture
    Capture* capture;
    ofTexture maskTex;
    ofPixels  maskPixels;

    ofImage maskImg;

    vector<Capture*> capturesAvailable;
    vector<Capture*> capturesActive;
    
    ofxSyphonServer mainOutputSyphonServer;
	  ofxSyphonServer individualTextureSyphonServer;


private:
    // helpers
    template <class T>
    void drawImageOnGrid(T img, string imgName, int i, int j, int gW, int gH, int pad);
    void scanDevices();

};

