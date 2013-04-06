#include "output.h"
#include "testApp.h"

outputWindow::outputWindow() {}

outputWindow::~outputWindow() {
	cout << "Image Listener destroyed" << endl;
}

void outputWindow::setup() {
	ofSetVerticalSync(true);
}

void outputWindow::draw(ofxFenster* f) {
    ofBackground(0);
    ofImage* t = &(((testApp*)ofGetAppPtr())->tgt);
    t->reloadTexture();
    t->draw(0,0, f->getWidth(), f->getHeight());
}


