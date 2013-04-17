#include "capture.h"

Capture::Capture(int deviceID, string deviceName, ofVideoGrabber& g)
                :id(deviceID),
                name(deviceName),
                vidGrabber(g)
{}

Capture::~Capture(){}

void Capture::setup(){

    capW = 320;
    capH = 240;
    framerate = 30;

    vidGrabber.setVerbose(true);
    vidGrabber.setPixelFormat(OF_PIXELS_RGB);
    //vidGrabber.setPixelFormat(OF_PIXELS_MONO);
    vidGrabber.setDesiredFrameRate(framerate);
    vidGrabber.initGrabber(capW,capH);

    colorImg.allocate(capW, capH);
    grayImage.allocate(capW, capH);
    grayBg.allocate(capW, capH);
    grayDiff.allocate(capW, capH);

}

void Capture::update(){

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame){

        colorImg.setFromPixels(vidGrabber.getPixels(), capW,capH);

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
    //grayDiff.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_NEAREST_NEIGHBOR);
}

void Capture::getAlphaMaskInverted(ofPixels* dst){
    //grayDiff.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_BICUBIC);
    grayDiff.invert();
    grayDiff.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_NEAREST_NEIGHBOR);
}
