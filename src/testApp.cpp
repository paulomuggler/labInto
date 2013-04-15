#include "testApp.h"
#include "proc_info.h"

//--------------------------------------------------------------
void testApp::setup()
{
    load_source_files();

    scanDevices();
    cout << capturesAvailable.size() << " captures available." << endl;
    capture = (capturesAvailable.back());
    cout << "selecting device " << capture->name << endl;
    capture->setup();

    loadSourceImg();
    configure_windows();

    threshold = 0;
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(100,100,100);
    capture->update();
    bool bNewFrame = capture->vidGrabber.isFrameNew();

    if (bNewFrame)
    {
        if(sourceImgChanged)
        {
            loadSourceImg();
            sourceImgChanged = false;
        }

        maskTargetImage();

    }
}

//--------------------------------------------------------------
void testApp::draw()
{

    // draw the incoming, the grayscale, the bg and the thresholded difference
    int gW = 320, gH = 240, pad = 10;

    drawImageOnGrid(capture->colorImg, "color image", 0, 0, gW, gH, pad);
    drawImageOnGrid(capture->grayImage, "grayscale image", 0, 1, gW, gH, pad);
    drawImageOnGrid(capture->grayBg, "background image", 1, 0, gW, gH, pad);
    drawImageOnGrid(capture->grayDiff, "alpha mask image", 1, 1, gW, gH, pad);
    drawImageOnGrid(srcImage, "source image", 2, 0, gW, gH, pad);

    ofEnableAlphaBlending();
    ofSetColor(0);
    ofRect(1*(gW+pad), 2*(gH+pad), gW, gH);
    ofSetColor(255);
    drawImageOnGrid(tgt, "output image", 2, 1, gW, gH, pad);
    ofDisableAlphaBlending();

    // finally, a report:

    double vm, rss;
    process_mem_usage(vm, rss);

    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
              << "press ' ' to capture bg" << endl
              << "threshold " << threshold << " (press: +/-)" << endl
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

// masks an image with a grayscale image
void testApp::maskTargetImage()
{

    tgt.setFromPixels(srcImage.getPixelsRef());
    tgt.setImageType(OF_IMAGE_COLOR_ALPHA);

    capture->getAlphaMask(&(maskImg.getPixelsRef()));
    maskImg.reloadTexture();

    unsigned char * maskPixels = maskImg.getPixels();
    unsigned char * destPixels = tgt.getPixels();

    int bpp = tgt.bpp / 8; //ofImage
    //int bpp = 3; //ofxCvColorImage

    //int w = srcImgW, h = srcImgH;
    int w = tgt.getWidth(), h = tgt.getHeight();

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {

            int maskPixel = maskPixels[i+(j*w)];

            destPixels[(j*w+i)*bpp+3] = maskPixel;
        }
    }
}

void testApp::configure_windows()
{
    // SETTING UP MULTIPLE WINDOWS WITH OFXFENSTER

    //IF the following code is uncommented, all the following windows should be created on the second display, if there is one available

    ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
    ofxDisplay* disp = displays[0];

    cout << "displays found: " << displays.size() << endl;

    if(displays.size() > 1)
        disp = displays[1];
    ofxFensterManager::get()->setActiveDisplay(disp);

    int winW=1024, winH=768;

    ow = new outputWindow();

    ofxFenster* win=ofxFensterManager::get()->createFenster(0, 0, winW, winH, OF_WINDOW);

    win->addListener(ow);
    win->setWindowTitle("output");

    //setup of fensterListener does not get called automatically yet
    ow->setup();

    // FINISH WINDOW SETUP
}

void testApp::load_source_files()
{
    //populate the sources directory object
    sourcesDir.open(sourcesPath);
    sourcesDir.listDir();

    ofLogNotice("source files found at "+sourcesPath+":");
    ofLogNotice(ofToString(sourcesDir.numFiles()));
    // done with file & image loading
}

void testApp::loadSourceImg()
{
    unsigned int f = fcursor%sourcesDir.numFiles();
    cout << "loading source image " << f << ": " << sourcesDir.getPath(f) << endl;
    srcImage.loadImage(sourcesDir.getPath(f));
    srcImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    srcImgW = srcImage.width;
    srcImgH = srcImage.height;
    cout << "resolution: " << srcImgW << "x" << srcImgW << endl;
    maskImg.clear();
    maskImg.allocate(srcImgW, srcImgH, OF_IMAGE_GRAYSCALE);
    tgt.clear();
    tgt.allocate(srcImgW, srcImgH, OF_IMAGE_COLOR_ALPHA);
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
    case '+':
        threshold ++;
        if (threshold > 255) threshold = 255;
        break;
    case '-':
        threshold --;
        if (threshold < 0) threshold = 0;
        break;
    case OF_KEY_LEFT:
        fcursor--;
        sourceImgChanged = true;
        break;
    case OF_KEY_RIGHT:
        fcursor++;
        sourceImgChanged = true;
        break;
    }
}

void testApp::scanDevices(){
    int i = 0,  deviceID = 0;;
    do{
        ofGstVideoGrabber* g = new ofGstVideoGrabber;
        g->setDeviceID(i);
        deviceID = g->deviceID;
        ofVideoGrabber* vg = (new ofVideoGrabber);
        vg->setGrabber(ofPtr<ofGstVideoGrabber>(g));
        Capture* c = new Capture(deviceID, g->camData.webcam_devices[deviceID].product_name, *vg);
        capturesAvailable.push_back(c);
    }while(i++ <= deviceID);

    // last device is a duplicate, deallocate it
    Capture* c = capturesAvailable.back();
    capturesAvailable.pop_back();
    //delete &(c->vidGrabber);
    //delete c;

}
