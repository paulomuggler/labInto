#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxFensterManager.h"

#include "output.h"
#include "capture.h"
#include "source.h"
#include "layer.h"

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

    // source image files
    unsigned int fcursor = UINT_MAX/2;
    string sourcesPath = "sources";
    ofDirectory sourcesDir;

    // image processing parameters
    int 				threshold;
    bool				bLearnBakground;

    // display parameters
    Source*			srcImage;
    int srcImgW, srcImgH;

    ofImage maskImg, tgt;

    // this window displays only the final image
    Layer* out;
    outputWindow* ow;

    vector<Capture*> capturesAvailable;
    vector<Capture*> capturesActive;


private:
    // helpers
    void configure_windows();
    void load_source_files();
    void loadSourceImg();
    template <class T>
    void drawImageOnGrid(T img, string imgName, int i, int j, int gW, int gH, int pad);
    bool sourceImgChanged = false;
    void scanDevices();

};

