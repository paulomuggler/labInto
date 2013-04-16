#include "layer.h"

Layer::Layer(Source* src, int w, int h)
    :source(src),
    width(w),
    height(h)
{}

Layer::~Layer() {}

void Layer::setup()
{
    //width = source->width, height = source->height;
    mPixMask.allocate(width,height, 1);
    mPixOut.allocate(width, height, 4);
    //mTex.allocate(width, height, GL_RGBA, true);
    mTex.allocate(width, height, GL_RGBA, true);
    maskTex.allocate(width, height, GL_LUMINANCE, true);

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
    string shaderProgram_alphaMask =
    "#version 120\n \
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

    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram_alphaMask);
    shader.linkProgram();

    // Let«s clear the FBO«s
    // otherwise it will bring some junk with it from the memory
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
}

void Layer::update()
{
    if(source->bNewFrame)
    {
        source->getFrame(&mPixOut);
        mTex.loadData(mPixOut);
    }

    if(fltrFlashLight->bNewFrame)
    {
        fltrFlashLight->getAlphaMaskInverted(&mPixMask);
        maskTex.loadData(mPixMask);
    }

    if(source->bNewFrame || fltrFlashLight->bNewFrame)
    {

        // HERE the shader-masking happens
        //
        ofEnableAlphaBlending();
        fbo.begin();
        // Cleaning everthing with alpha mask on 0 in order to make it transparent for default
        ofClear(0, 0, 0, 255);

        shader.begin();

        shader.setUniformTexture("maskTex", maskTex, 1 );
        mTex.draw(0,0);

        shader.end();
        fbo.end();
        ofDisableAlphaBlending();
    }

}

void Layer::draw()
{
    this->draw(0, 0, width, height);
}

void Layer::draw(int x1, int y1, int w, int h)
{
        ofEnableAlphaBlending();
        ofSetColor(0);
        ofRect(x1,y1,w,h);
        ofSetColor(255);
        mTex.draw(x1,y1, w, h);
        ofSetColor(0);
        fbo.draw(x1,y1, w, h);
        ofDisableAlphaBlending();
}

void Layer::getFrame(ofPixels* dst)
{
    mPixOut.resizeTo(*dst, OF_INTERPOLATE_BICUBIC);
}
