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
    string sourcesPath = "pictures";
    ofDirectory sourcesDir;

    // source
    Source*			source;
    Source*			srcImage;
    Source*			srcVideo;
    int srcImgW, srcImgH;
    ofTexture srcTex;

    ofImage maskImg;

    // this window displays only the final image
    Layer* out;
    ofxFenster* win;
    outputWindow* ow;

    vector<Capture*> capturesAvailable;
    vector<Capture*> capturesActive;


private:
    // helpers
    void configure_windows();
    void load_source_files();
    void loadSource();
    template <class T>
    void drawImageOnGrid(T img, string imgName, int i, int j, int gW, int gH, int pad);
    bool sourceChanged = false;
    void scanDevices();

};


