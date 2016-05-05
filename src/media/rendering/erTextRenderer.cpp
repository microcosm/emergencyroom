#include "erTextRenderer.h"

void erTextRenderer::setup(){
    ofAddListener(ofEvents().draw, this, &erTextRenderer::draw);
    masker.setup(ofClamp(ofGetWidth(), 0, 900), 900);
    masker.newLayer();
    masker.toggleOverlay();
}

void erTextRenderer::draw(ofEventArgs& args){
    if(currentTexts != NULL){
        if(withinOverlayPeriod()){
            ofSetColor(ofColor::black, 46);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        }
        if(withinTextPeriod()){
            drawText();
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

void erTextRenderer::drawText(){
    masker.beginLayer();
    {
        ofSetColor(ofColor::white);
        int i = 0;
        for(auto text : *currentTexts) {
            ofDrawBitmapString(text, 50, i+=50);
        }
    }
    masker.endLayer();

    masker.beginMask();
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        ofDrawRectangle(0, 0, 800, 240);
    }
    masker.endMask();

    masker.draw(0, 40);
    if(settings.clientDrawingEnabled){
        masker.drawOverlay();
    }
}