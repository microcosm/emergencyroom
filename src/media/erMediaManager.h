#pragma once
#include "ofMain.h"
#include "erMedia.h"
#include "erMediaRenderer.h"
#include "erNetwork.h"
#include "erPlayParams.h"
#include "erSeaLevelViz.h"

class erMediaManager{

public:
    void setup(int numChannels, erNetwork* network);
    void play(erPlayParams params);
    void render(erPlayParams params);
    vector<string> getVideoCollections();
    map<string,vector<string>> getCollectionsToVideos();
    vector<string> getAllVideos();
protected:
    erMedia media;
    erMediaRenderer renderer;
    erSeaLevelViz seaLevelViz;
};