#pragma once
#include "ofMain.h"
#include "erMedia.h"
#include "erPlayParams.h"
#include "erSeaLevelViz.h"

class erMediaManager{

public:
    void setup();
    void play(erPlayParams params);
    void render(erPlayParams params);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    vector<string> getVideoCollections();
    map<string,vector<string>> getCollectionsToVideos();
    vector<string> getAllVideos();
protected:
    erMedia media;
    erSeaLevelViz seaLevelViz;
};