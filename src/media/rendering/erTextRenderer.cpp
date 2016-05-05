#include "erTextRenderer.h"

void erTextRenderer::setup(){
    ofAddListener(ofEvents().draw, this, &erTextRenderer::draw);
}

void erTextRenderer::draw(ofEventArgs& args){
    if(currentTexts != NULL && !settings.clientDrawingEnabled){
        if(withinOverlayPeriod()){
            ofSetColor(ofColor::black, 46);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        }
        if(withinTextPeriod()){
            ofSetColor(ofColor::white);
            int i = 0;
            for(auto text : *currentTexts) {
                ofDrawBitmapString(text, 50, i+=50);
            }
        }
    }
}

void erTextRenderer::setTexts(map<string, vector<string>>* _texts){
    texts = _texts;
}

void erTextRenderer::newTextPeriod(u_int64_t from, float duration, erPlayParams params){
    startTextAt = from;
    endTextAt = from + duration;
    currentTexts = &texts->at(params.getPath());
}

void erTextRenderer::newOverlayPeriod(u_int64_t from, float duration){
    startOverlayAt = from;
    endOverlayAt = from + duration;
}

bool erTextRenderer::withinTextPeriod(){
    now = ofGetElapsedTimeMillis();
    return now > startTextAt && now < endTextAt;
}

bool erTextRenderer::withinOverlayPeriod(){
    now = ofGetElapsedTimeMillis();
    return now > startOverlayAt && now < endOverlayAt;
}