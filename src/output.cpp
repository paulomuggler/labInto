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
    setup_shader();
}

void outputWindow::update(ofxFenster* f)
{

    // HERE the shader-masking happends
    //
    ofEnableAlphaBlending();
    fbo.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent for default
    ofClear(0, 0, 0, 0);

    //shader.begin();
    //shader.setUniformTexture("maskTex", (((testApp*)ofGetAppPtr())->gryDiff).getTextureReference(), 1 );

    (((testApp*)ofGetAppPtr())->srcImage).draw(0,0);

    //shader.end();
    fbo.end();
    ofDisableAlphaBlending();

}

void outputWindow::draw(ofxFenster* f)
{
    ofSetColor(255,255);

   // dstImg.draw(0,0);

    fbo.draw(0,0);
//    ofBackground(0);
//    ofImage* source = &(((testApp*)ofGetAppPtr())->srcImage);
//    ofImage* maskImg = &(((testApp*)ofGetAppPtr())->gryDiff);
//
//    source->reloadTexture();
//
//    ofSetColor(255);
//
//    ofTexture mask = source->getTextureReference();
//    int w = f->getWidth(), h = f->getHeight();
//
//    mask.bind();
//
//    float vertexData[]=
//    {
//        0.0f, 0.0f,
//        w, 0.0f,
//        0.0f, h,
//        w, h
//    };
//
//    float texCoordData[]=
//    {
//        0.0f, 0.0f,
//        0.5f, 0.0f,
//        0.0f, 0.5f,
//        0.5f, 0.5f
//    };
//
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    glVertexPointer(2,GL_FLOAT, 0, vertexData);
//    glTexCoordPointer(2,GL_FLOAT, 0, texCoordData);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//    mask.unbind();

//   target->reloadTexture();
//   target->draw(0,0, f->getWidth(), f->getHeight());
}


void outputWindow::setup_shader()
{
    int width =   (((testApp*)ofGetAppPtr())->srcImage).getWidth();
    int height =  (((testApp*)ofGetAppPtr())->srcImage).getHeight();

    fbo.allocate(width,height);

    ofSetWindowShape(width, height);


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
    float mask = texture2DRect(maskTex, pos).r;\
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



