#include "output.h"
#include "testApp.h"

outputWindow::outputWindow() {}

outputWindow::~outputWindow()
{
    cout << "Image Listener destroyed" << endl;
}

void outputWindow::setup(ofxFenster* f)
{
    ofSetVerticalSync(true);
}

void outputWindow::update(ofxFenster* f)
{

}

ofPixels out;
ofTexture oTex;
void outputWindow::draw(ofxFenster* f)
{
    ofFbo fb = (((testApp*)ofGetAppPtr())->fbo);
    oTex.allocate(fb.getWidth(),fb.getHeight(), OF_IMAGE_COLOR);
    out.allocate(fb.getWidth(),fb.getHeight(), OF_IMAGE_COLOR);
    fb.readToPixels(out);
    ofSetColor(255,255);
    (((testApp*)ofGetAppPtr())->srcImage).reloadTexture();
//    (((testApp*)ofGetAppPtr())->fbo).reloadTexture();

    oTex.loadData(out);

    (((testApp*)ofGetAppPtr())->srcImage).draw(0,0);
    oTex.draw(0,0);
    //(((testApp*)ofGetAppPtr())->fbo).draw(0,0);

}


