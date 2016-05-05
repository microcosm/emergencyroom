#pragma once
#include "ofMain.h"
#include "erPlayParams.h"
#include "erSyncedVideoPlayer.h"
#include "erSettings.h"
#include "erNetwork.h"

class erTextRenderer{

public:
    void setup();
    virtual void draw(ofEventArgs& args);
    void setTexts(map<string, vector<string>>* _texts);
    void newTextPeriod(u_int64_t from, float duration, erPlayParams params);
protected:
    bool withinGlitchPeriod();
    map<string, vector<string>>* texts;
    vector<string>* currentTexts;
    ofPtr<erSyncedVideoPlayer> currentVideoPlayer;
    u_int64_t now, startAt, endAt;
    int videoDuration, padding;
    erNetwork* network;
};