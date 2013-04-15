#include "capture.h"

Capture::Capture(int deviceID, string deviceName, ofVideoGrabber& g)
                :id(deviceID),
                name(deviceName),
                vidGrabber(g)
{}

Capture::~Capture(){}

void Capture::setup(){

    vidGrabber.setVerbose(true);
    vidGrabber.setPixelFormat(OF_PIXELS_RGB);
    //vidGrabber.setPixelFormat(OF_PIXELS_MONO);
    vidGrabber.setDesiredFrameRate(50);
    vidGrabber.initGrabber(320,240);

}

void Capture::update(){

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame){

        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);

        grayImage = colorImg;

        if (bLearnBakground == true)
        {
            grayBg = grayImage;
            bLearnBakground = false;
        }

        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.blur(21);
        //grayDiff.threshold(threshold);
        IplImage* cvImage = grayDiff.getCvImage();
        //cvThreshold(cvImage, cvImage, threshold, 0, CV_THRESH_TOZERO);
        cvErode(cvImage, cvImage, NULL, 1);
        cvDilate(cvImage, cvImage, NULL, 1);
        //grayDiff.contrastStretch();
        //grayDiff.dilate_3x3();
        //grayDiff.erode_3x3();
        //grayDiff.blurGaussian(33);
        //grayDiff.adaptiveThreshold(threshold, 0 ,false, true);
    }

}

void Capture::learnBackground(){
    bLearnBakground = true;
}

void Capture::getAlphaMask(ofPixels* dst){
    grayDiff.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_BICUBIC);
}
