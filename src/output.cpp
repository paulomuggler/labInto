#include "output.h"
#include "testApp.h"

outputWindow::outputWindow(Layer* lyr):outLayer(lyr) {}

outputWindow::~outputWindow() {
	cout << "Image Listener destroyed" << endl;
}

void outputWindow::setup() {
	ofSetVerticalSync(true);
    out.allocate(outLayer->mPixOut);
}

void outputWindow::update() {
    out.loadData(outLayer->mPixOut);
}

void outputWindow::draw(ofxFenster* f) {
    ofBackground(0);
    ofEnableAlphaBlending();
//    outLayer->draw(0,0, f->getWidth(), f->getHeight());
    out.draw(0,0, f->getWidth(), f->getHeight());
    ofDisableAlphaBlending();
}


