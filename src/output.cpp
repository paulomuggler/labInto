#include "output.h"
#include "testApp.h"

outputWindow::outputWindow() {}
outputWindow::outputWindow(Layer* lyr):outLayer(lyr) {}

ofFbo fbo;
outputWindow::~outputWindow() {
	cout << "Image Listener destroyed" << endl;
}

void outputWindow::setup() {
	ofSetVerticalSync(true);

    outLayer = new Layer(((testApp*)ofGetAppPtr())->srcImage, 1024, 768);
    outLayer->setup();
    outLayer->fltrFlashLight = ((testApp*)ofGetAppPtr())->capture;

}

void outputWindow::update() {
    outLayer->update();
}

void outputWindow::draw(ofxFenster* f) {
    ofBackground(0);
    ofEnableAlphaBlending();
    outLayer->draw(0,0, f->getWidth(), f->getHeight());
    ofDisableAlphaBlending();
}


