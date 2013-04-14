#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"

class outputWindow: public ofxFensterListener
{

public:
    outputWindow();
    ~outputWindow();

    void setup();
    void draw(ofxFenster* f);

    void keyReleased(int key, ofxFenster* window)
    {
        cout << "key pressed: " << key << endl;
     //   ((testApp*)ofGetAppPtr())->keyPressed(key);
    }

};
