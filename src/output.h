#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"
#include "layer.h"

class outputWindow: public ofxFensterListener
{

public:
    outputWindow(Layer* lyr);
    outputWindow();
    ~outputWindow();

    void setup();
    void update();
    void draw(ofxFenster* f);

    Layer* outLayer;
    ofTexture out;

    void keyReleased(int key, ofxFenster* window)
    {
        cout << "key pressed: " << key << endl;
     //   ((testApp*)ofGetAppPtr())->keyPressed(key);
    }

};
