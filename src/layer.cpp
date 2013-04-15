#include "layer.h"

Layer::Layer(Source* src)
    :source(src)
{}

Layer::~Layer() {}

void Layer::setup()
{
    //width = source->width, height = source->height;
    width = 512, height = 512;
    mPixMask.allocate(width,height, 1);
    mPixOut.allocate(width, height, 4);
    //mTex.allocate(width, height, GL_RGBA, true);
    mTex.allocate(mPixOut);
}

void Layer::update()
{
    if(source->bNewFrame || fltrFlashLight->bNewFrame)
    {

        source->getFrame(&mPixOut);
        //mPixOut = source->srcImg.getPixelsRef();
        fltrFlashLight->getAlphaMask(&mPixMask);
        mPixOut.setChannel(4, mPixMask);
        mTex.loadData(mPixOut);

    }

}

void Layer::draw()
{
    this->draw(0, 0, width, height);
}

void Layer::draw(int x1, int y1, int x2, int y2)
{
    if(source->bNewFrame || fltrFlashLight->bNewFrame)
    {
        ofEnableAlphaBlending();
        ofSetColor(0);
        ofRect(x1, y1, x2, y2);
        ofSetColor(255);
        mTex.draw(x1, y1, x2, y2);
        ofDisableAlphaBlending();
    }
}

void Layer::getFrame(ofPixels* dst)
{
    mPixOut.resizeTo(*dst, OF_INTERPOLATE_BICUBIC);
}
