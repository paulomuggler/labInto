#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxFensterManager.h"

#include "output.h"
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

    void maskTargetImage();


    // video capture
    ofVideoGrabber 		vidGrabber;
    bool bNewFrame = false;

    // source image files
    unsigned int fcursor = UINT_MAX/2;
    string sourcesPath = "sources";
    ofDirectory sourcesDir;
    ofImage			srcImage;

    // image processing parameters
    int 				threshold;
    bool				bLearnBakground;

    // display parameters
    int srcImgW, srcImgH;

    // ofxOpenCv image types for processing
    ofxCvColorImage			colorImg;

    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    ofImage maskImg; // this will be the same size as srcImage

    ofImage tgt;

    // this window displays only the final image
    outputWindow* ow;

    vector<GrabberDevice> capturesAvailable;
    vector<GrabberDevice> capturesActive;
    void scanDevices();


private:
    // helpers
    void configure_windows();
    void load_source_files();
    void loadSourceImg();
    template <class T>
    void drawImageOnGrid(T img, string imgName, int i, int j, int gW, int gH, int pad);
    bool sourceImgChanged = false;

};

