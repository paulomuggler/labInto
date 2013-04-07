#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{

    load_files();
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);

    loadSourceImg();

    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);


    bLearnBakground = true;
    threshold = 80;

    setup_shader();
    //configure_windows();
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(100,100,100);

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame)
    {
        bNewFrame = false;
        if(sourceImgChanged)
        {
            loadSourceImg();
            sourceImgChanged = false;
        }

        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);

        grayImage = colorImg;
        if (bLearnBakground == true)
        {
            grayBg = grayImage;
            bLearnBakground = false;
        }

        // take the abs value of the difference between background and incoming and then threshold:
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);

        maskTargetImage();

        grayDiff.getPixelsRef().resizeTo(maskImg.getPixelsRef(), OF_INTERPOLATE_BICUBIC);
        maskImg.reloadTexture();

        // HERE the shader-masking happens
        //
        ofEnableAlphaBlending();
        fbo.begin();
        // Cleaning everthing with alpha mask on 0 in order to make it transparent for default
        ofClear(0, 0, 0, 0);

        shader.begin();

        shader.setUniformTexture("maskTex", maskImg.getTextureReference(), 1 );
        srcImage.draw(0,0);

        shader.end();
        fbo.end();
        ofDisableAlphaBlending();

    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    //tgt.reloadTexture();

    // copy a bunch of ofxCvImage types to ofImage ones because ofxCv and ofImage are not playing nice
    clr.setFromPixels(colorImg.getPixelsRef());
    gry.setFromPixels(grayImage.getPixelsRef());
    gryBg.setFromPixels(grayBg.getPixelsRef());
    gryDiff.setFromPixels(grayDiff.getPixelsRef());

    // draw the incoming, the grayscale, the bg and the thresholded difference
    int gW = 320, gH = 240, pad = 10;

    drawImageOnGrid(clr, "color image", 0, 0, gW, gH, pad);
    drawImageOnGrid(gry, "grayscale image", 0, 1, gW, gH, pad);
    drawImageOnGrid(gryBg, "background image", 1, 0, gW, gH, pad);
    drawImageOnGrid(gryDiff, "grayscale image minus background\n(diff & threshold)", 1, 1, gW, gH, pad);
    drawImageOnGrid(srcImage, "source image", 2, 0, gW, gH, pad);
    drawImageOnGrid(tgt, "output image", 2, 1, gW, gH, pad);

    // finally, a report:
    ofSetHexColor(0xffff00);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
              << "press ' ' to capture bg" << endl
              << "threshold " << threshold << " (press: +/-)" << endl
              << ", fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 660, 20);

    ofSetColor(255,255);

    srcImage.draw(800,0);
    fbo.draw(800,0);

}

void testApp::drawImageOnGrid(ofImage img, string imgName, int row, int col, int gW, int gH, int pad)
{
    int i = col, j = row;
    ofSetHexColor(0xffffff);
    img.draw(i*gW+i*pad, j*gH+j*pad, gW, gH);

    ofSetHexColor(0xffff00);
    stringstream reportStr;
    reportStr << imgName << endl;
    ofDrawBitmapString(reportStr.str(), i*gW+2*i*pad, j*gH+2*(j+1)*pad);
}

ofImage src;
// masks an image with a grayscale image
void testApp::maskTargetImage()
{

    if(!src.bAllocated()){
        src.allocate(320,240, OF_IMAGE_COLOR);
    }

    //grayDiff.blurGaussian(33);

    srcImage.getPixelsRef().resizeTo(src.getPixelsRef(), OF_INTERPOLATE_NEAREST_NEIGHBOR);

    tgt.setFromPixels(src.getPixelsRef());

    unsigned char * maskPixels = grayDiff.getPixels();
    unsigned char * srcPixels = src.getPixels();
    unsigned char * destPixels = tgt.getPixels();

    int bpp = tgt.bpp / 8; //ofImage
    //int bpp = 3; //ofxCvColorImage

    int w = src.getWidth(), h = src.getHeight();


    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {

            int maskPixel = maskPixels[i+(j*w)];
            float mAlpha = ofMap(maskPixel, 0, 255, 1.0, 0.25);
            //float mAlpha = abs(log(ofMap(maskPixel, 0, 255, 0, 1.0)));

            // code for ofImage pixel access

            destPixels[(j*w+i)*bpp+0] = mAlpha*maskPixel + (1-mAlpha)*srcPixels[(j*w+i)*bpp+0];
            destPixels[(j*w+i)*bpp+1] = mAlpha*maskPixel + (1-mAlpha)*srcPixels[(j*w+i)*bpp+1];
            destPixels[(j*w+i)*bpp+2] = mAlpha*maskPixel + (1-mAlpha)*srcPixels[(j*w+i)*bpp+2];
        }
    }
}

void testApp::configure_windows()
{
    // SETTING UP MULTIPLE WINDOWS WITH OFXFENSTER

    //IF the following code is uncommented, all the following windows should be created on the second display, if there is one available

    ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
    ofxDisplay* disp = displays[0];

    cout << "displays found: " << displays.size() << endl;

    if(displays.size() > 1)
        disp = displays[1];
    ofxFensterManager::get()->setActiveDisplay(disp);

    int winW=1024, winH=768;

    ow = new outputWindow();

    ofxFenster* win=ofxFensterManager::get()->createFenster(0, 0, winW, winH, OF_WINDOW);

    win->addListener(ow);
    win->setWindowTitle("output");

    //setup of fensterListener does not get called automatically yet
    ow->setup(win);

    // FINISH WINDOW SETUP
}

void testApp::setup_shader()
{
    int width =   srcImage.getWidth();
    int height =  srcImage.getHeight();

    fbo.allocate(width,height);

    //ofSetWindowShape(width, height);

    // There are 3 of ways of loading a shader:
    //
    //  1 - Using just the name of the shader and ledding ofShader look for .frag and .vert:
    //      Ex.: shader.load( "myShader");
    //
    //  2 - Giving the right file names for each one:
    //      Ex.: shader.load( "myShader.vert","myShader.frag");
    //
    //  3 - And the third one it«s passing the shader programa on a single string;
    //
    string shaderProgram = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect tex0;\
    uniform sampler2DRect maskTex;\
    \
    void main (void){\
    vec2 pos = gl_TexCoord[0].st;\
    \
    vec3 src = texture2DRect(tex0, pos).rgb;\
    float mask = 0.666*1/texture2DRect(maskTex, pos).r;\
    \
    gl_FragColor = vec4( src , mask);\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();

    // Let«s clear the FBO«s
    // otherwise it will bring some junk with it from the memory
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();

}

void testApp::load_files()
{
    //populate the sources directory object
    sourcesDir.open(sourcesPath);
    sourcesDir.listDir();

    ofLogNotice("source files found at "+sourcesPath+":");
    ofLogNotice(ofToString(sourcesDir.numFiles()));

    // done with file & image loading
}

void testApp::loadSourceImg()
{
    unsigned int f = fcursor%sourcesDir.numFiles();
    cout << "loading source image " << f << ": " << sourcesDir.getPath(f) << endl;
    srcImage.loadImage(sourcesDir.getPath(f));
    srcImgW = srcImage.width;
    srcImgH = srcImage.height;
    maskImg.allocate(srcImgW, srcImgH, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    cout << "key pressed: " << key << endl;
    switch (key)
    {
    case ' ':
        bLearnBakground = true;
        break;
    case '+':
        threshold ++;
        if (threshold > 255) threshold = 255;
        break;
    case '-':
        threshold --;
        if (threshold < 0) threshold = 0;
        break;
    case OF_KEY_LEFT:
        fcursor--;
        sourceImgChanged = true;
        break;
    case OF_KEY_RIGHT:
        fcursor++;
        sourceImgChanged = true;
        break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
