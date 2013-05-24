#include "source.h"

Source::Source(string src, resourceType type)
                :path(src),
                mtype(type)
{
    switch(mtype)
    {
        case SRC_SOLIDCOLOR:
            // create color
            break;
        case SRC_IMAGE:
            setup_func = &Source::setup_image;
            update_func = &Source::update_image;
            getFrame_func = &Source::getFrame_image;
            break;
        case SRC_VIDEO:
            setup_func = &Source::setup_video;
            update_func = &Source::update_video;
            getFrame_func = &Source::getFrame_video;
            break;

    }
}


Source::~Source(){}

void Source::setup(){
    //(obj.*obj.pf)(1, 2)
    (*this.*this->setup_func)();
}

void Source::update(){
    (*this.*this->update_func)();
}

void Source::getFrame(ofPixels* dst){
    (*this.*this->getFrame_func)(dst);
}

void Source::setup_image(){
    cout << "loading source image: " << path << endl;
    srcImg.setImageType(OF_IMAGE_COLOR_ALPHA);
    srcImg.loadImage(path);
    width = srcImg.getWidth();
    height = srcImg.getHeight();
    cout << "resolution: " << width << "x" << height << endl;
    source = &srcImg;
    bNewFrame = true;
}
void Source::update_image(){
    //setup_image();
}
void Source::getFrame_image(ofPixels* dst){
    srcImg.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_BICUBIC);
    //srcImg.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_NEAREST_NEIGHBOR);
    bNewFrame = false;
}

void Source::setup_video(){
    cout << "loading source video: " << path << endl;
    srcVideo.setPixelFormat(OF_PIXELS_RGBA);
    srcVideo.loadMovie(path);
    srcVideo.play();
    width = srcVideo.getWidth();
    height = srcVideo.getHeight();
    cout << "resolution: " << width << "x" << height << endl;
    source = &srcVideo;
    bNewFrame = true;
}
void Source::update_video(){
    srcVideo.update();
    bNewFrame = srcVideo.isFrameNew();
}
void Source::getFrame_video(ofPixels* dst){
    srcVideo.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_BICUBIC);
    //srcVideo.getPixelsRef().resizeTo(*dst, OF_INTERPOLATE_NEAREST_NEIGHBOR);
    bNewFrame = false;
}
