#include "erTextRenderer.h"

void erTextRenderer::setup(){
    ofAddListener(ofEvents().draw, this, &erTextRenderer::draw);
}

void erTextRenderer::draw(ofEventArgs& args){
    if(currentTexts != NULL && !settings.clientDrawingEnabled && withinGlitchPeriod()){
        ofSetColor(ofColor::black, 50);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(ofColor::white);
        int i = 0;
        for(auto text : *currentTexts) {
            ofDrawBitmapString(text, 50, i+=50);
        }
    }
}

void erTextRenderer::setTexts(map<string, vector<string>>* _texts){
    texts = _texts;
}

void erTextRenderer::newTextPeriod(u_int64_t from, float duration, erPlayParams params){
    startAt = from;
    endAt = from + duration;
    currentTexts = &texts->at(params.getPath());
}

bool erTextRenderer::withinGlitchPeriod(){
    now = ofGetElapsedTimeMillis();
    return now > startAt && now < endAt;
}