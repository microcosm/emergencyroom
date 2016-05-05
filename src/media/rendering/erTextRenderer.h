#pragma once
#include "ofMain.h"
#include "erPlayParams.h"
#include "erSyncedVideoPlayer.h"

class erTextRenderer{

public:
    void setup();
    virtual void draw(ofEventArgs& args);
    void setTexts(map<string, vector<string>>* _texts);
    void execute(erPlayParams params, ofPtr<erSyncedVideoPlayer> _videoPlayer);
protected:
    map<string, vector<string>>* texts;
    vector<string>* currentTexts;
    ofPtr<erSyncedVideoPlayer> currentVideoPlayer;
};