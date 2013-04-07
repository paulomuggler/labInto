#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

class outputWindow: public ofxFensterListener
{

public:
    outputWindow();
    ~outputWindow();

    void setup(ofxFenster* f);
    void update(ofxFenster* f);
    void draw(ofxFenster* f);

    void keyReleased(int key, ofxFenster* window)
    {
        cout << "key pressed: " << key << endl;
        //   ((testApp*)ofGetAppPtr())->keyPressed(key);
    }

private:
    // helpers


};
