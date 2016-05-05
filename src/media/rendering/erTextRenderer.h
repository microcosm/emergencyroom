#pragma once
#include "ofMain.h"
#include "erPlayParams.h"
#include "erSyncedVideoPlayer.h"
#include "erSettings.h"
#include "erNetwork.h"
#include "ofxLayerMask.h"

class erTextRenderer{

public:
    void setup();
    virtual void draw(ofEventArgs& args);
    void setTexts(map<string, vector<string>>* _texts);
    void newTextPeriod(u_int64_t from, float duration, erPlayParams params);
    void newOverlayPeriod(u_int64_t from, float duration);
protected:
    bool withinTextPeriod();
    bool withinOverlayPeriod();
    void drawText();
    map<string, vector<string>>* texts;
    vector<string>* currentTexts;
    ofPtr<erSyncedVideoPlayer> currentVideoPlayer;
    u_int64_t now, startTextAt, endTextAt, startOverlayAt, endOverlayAt;
    int videoDuration, padding;
    erNetwork* network;
    ofxLayerMask masker;
};