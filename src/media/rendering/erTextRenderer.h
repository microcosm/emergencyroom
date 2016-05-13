#pragma once
#include "ofMain.h"
#include "erPlayParams.h"
#include "erSettings.h"
#include "erNetwork.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"

class erTextRenderer{

public:
    void setup();
    void update();
    void draw();
    void setTexts(map<string, vector<string>>* _texts);
    void newTextPeriod(u_int64_t from, float duration, erPlayParams params);
    void newOverlayPeriod(u_int64_t from, float duration);
protected:
    bool withinTextPeriod();
    bool withinOverlayPeriod();
    bool withinAnimationPeriod();
    void drawText();

    int animationPeriodLength, textHeight, indentX, numLines, textOffset;
    ofxAnimatableFloat textY;
    map<string, vector<string>>* texts;
    vector<string>* currentTexts;
    u_int64_t now, startTextAt, endTextAt, startOverlayAt, endOverlayAt, nextAnimationBeginsAt;
    int videoDuration, padding;
    erNetwork* network;
    ofxLayerMask masker;
};