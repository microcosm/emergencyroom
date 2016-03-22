#pragma once
#include "ofMain.h"
#include "erMedia.h"

class erMediaRenderer{

public:
    void setup(erMedia* _media);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    
protected:
    erMedia* media;
};