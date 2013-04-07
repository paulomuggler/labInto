#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxFensterManager.h"

#include "output.h"

//#define _USE_LIVE_VIDEO

class testApp : public ofBaseApp
{

public:

    // application lifecycle methods
    void setup();
    void update();
    void draw();

    // UI methods
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

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

    // display pipeline variables
    ofFbo       fbo;
    ofShader    shader;

    // ofxOpenCv image types for processing
    ofxCvColorImage			colorImg;

    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;

    ofImage maskImg;

    // ofImage types for displaying since ofxFenster is not playing nice with ofxCvImage
    ofImage clr, gry, gryBg, gryDiff, tgt;

    // this window displays only the final image
    outputWindow* ow;


private:
    // helpers
    void setup_shader();
    void configure_windows();
    void load_files();
    void loadSourceImg();
    void drawImageOnGrid(ofImage img, string imgName, int i, int j, int gW, int gH, int pad);
    bool sourceImgChanged = false;

};

