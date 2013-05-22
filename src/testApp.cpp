#include "testApp.h"
#include "proc_info.h"

//--------------------------------------------------------------
void testApp::setup()
{

    scanDevices();
    cout << capturesAvailable.size() << " captures available." << endl;
    capture = (capturesAvailable.front());
    cout << "selecting device " << capture->name << endl;
    capture->setup();
    
    maskPixels.allocate(capture->capW, capture->capH, 1);
    maskTex.allocate(maskPixels);
    
    mainOutputSyphonServer.setName("Screen Output");
	individualTextureSyphonServer.setName("Texture Output");
    
    ofSetFrameRate(60);

}

//--------------------------------------------------------------
void testApp::update()
{
    capture->update();
    bool bNewFrame = capture->vidGrabber.isFrameNew();

    if (bNewFrame)
    {

    }
}

//--------------------------------------------------------------
void testApp::draw()
{

    ofBackground(100,100,100);
    // draw the incoming, the grayscale, the bg and the thresholded difference
    int gW = 320, gH = 240, pad = 10;

    drawImageOnGrid(capture->colorImg, "color image", 0, 0, gW, gH, pad);
    drawImageOnGrid(capture->grayImage, "grayscale image", 0, 1, gW, gH, pad);
    drawImageOnGrid(capture->grayBg, "background image", 1, 0, gW, gH, pad);
    drawImageOnGrid(capture->grayDiff, "alpha mask image", 1, 1, gW, gH, pad);
    
    // Syphon Stuff
    
    ofSetColor(255, 255, 255);
    
    ofEnableAlphaBlending();
    
	mainOutputSyphonServer.publishScreen();
    capture->getAlphaMask(&maskPixels);
    maskTex.loadData(maskPixels);
    individualTextureSyphonServer.publishTexture(&maskTex);


    // finally, a report:

    double vm, rss;
    process_mem_usage(vm, rss);

    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
              << "press ' ' to capture bg" << endl
              << ", fps: " << ofGetFrameRate() << endl
              << "VM: " << vm << "; RSS: " << rss << endl;
    ofDrawBitmapString(reportStr.str(), 660, 20);

}


template <class T>
void testApp::drawImageOnGrid(T img, string imgName, int row, int col, int gW, int gH, int pad){
    int i = col, j = row;
    ofSetHexColor(0xffffff);
    img.draw(i*gW+i*pad, j*gH+j*pad, gW, gH);

    ofSetHexColor(0xffff00);
    stringstream reportStr;
    reportStr << imgName << endl;
    ofDrawBitmapString(reportStr.str(), i*gW+2*i*pad, j*gH+2*(j+1)*pad);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    cout << "key pressed: " << key << endl;
    switch (key)
    {
    case ' ':
        capture->learnBackground();
        break;
    }
}


void testApp::scanDevices(){
    int i = 0;
    do{
        ofVideoGrabber* vg = (new ofVideoGrabber);
        Capture* c = new Capture(i, "capture"+ofToString(i), *vg);
        capturesAvailable.push_back(c);
    }while(i++ <= 2);
}
 
