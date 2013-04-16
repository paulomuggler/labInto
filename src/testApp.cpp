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

    srcImage = new Source("", SRC_IMAGE);
    loadSourceImg();

    out = new Layer(srcImage, 1024, 768);
    out->setup();
    out->fltrFlashLight = capture;

    configure_windows();
}

//--------------------------------------------------------------
void testApp::update()
{
    capture->update();
    bool bNewFrame = capture->vidGrabber.isFrameNew();

    if (bNewFrame)
    {
        out->update();
        if(sourceImgChanged)
        {
            loadSourceImg();
            sourceImgChanged = false;
        }
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
    drawImageOnGrid(srcTex, "source image", 2, 0, gW, gH, pad);

    out -> draw(800, 0, out->width, out->height);
    //out -> draw(0, 0, 640, 480);

    out -> draw(330, 500, 320, 240);

    //drawImageOnGrid(*out, "output image", 2, 1, gW, gH, pad);

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

    ow = new outputWindow(out);

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
}

void testApp::loadSourceImg()
{
    unsigned int f = fcursor%sourcesDir.numFiles();
    srcImage->path = sourcesDir.getPath(f);
    srcImage->update();
    srcImgW = srcImage->width;
    srcImgH = srcImage->height;
    maskImg.clear();
    maskImg.allocate(srcImgW, srcImgH, OF_IMAGE_GRAYSCALE);
    srcTex.allocate(srcImage->srcImg.getPixelsRef());
    srcTex.loadData(srcImage->srcImg.getPixelsRef());
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
    delete &(c->vidGrabber);
    delete c;

}
