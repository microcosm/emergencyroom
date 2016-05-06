#include "erTextRenderer.h"

void erTextRenderer::setup(){
    animationPeriodLength = 500;
    textHeight = 16;
    indentX = 24;
    numLines = 3;
    textOffset = -4;
    textY.setRepeatType(PLAY_ONCE);
    textY.setCurve(EASE_IN_EASE_OUT);
    textY.setDuration(0.1);
    masker.setup(ofClamp(ofGetWidth(), 0, 900), 900);
    masker.newLayer();
    masker.toggleOverlay();
    ofAddListener(ofEvents().draw, this, &erTextRenderer::draw);
    ofAddListener(ofEvents().update, this, &erTextRenderer::update);
}

void erTextRenderer::update(ofEventArgs& args){
    now = ofGetElapsedTimeMillis();
    textY.update(ofGetLastFrameTime());
}

void erTextRenderer::draw(ofEventArgs& args){
    if(currentTexts != NULL){
        now = ofGetElapsedTimeMillis();

        if(withinOverlayPeriod()){
            ofSetColor(ofColor::black, 46);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        }

        if(withinTextPeriod()){
            drawText();

            if(withinAnimationPeriod()){
                textY.animateTo(textY.val() - textHeight);
                nextAnimationBeginsAt += animationPeriodLength;
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
    nextAnimationBeginsAt = startTextAt + animationPeriodLength;

    int randomLineOffset = ofClamp(floor(ofRandom(currentTexts->size() - 5)), 0, 1000);
    textY.reset(textOffset + -(randomLineOffset * textHeight));
}

void erTextRenderer::newOverlayPeriod(u_int64_t from, float duration){
    startOverlayAt = from;
    endOverlayAt = from + duration;
}

bool erTextRenderer::withinTextPeriod(){
    return now > startTextAt && now < endTextAt;
}

bool erTextRenderer::withinOverlayPeriod(){
    return now > startOverlayAt && now < endOverlayAt;
}

bool erTextRenderer::withinAnimationPeriod(){
    return now >= nextAnimationBeginsAt;
}

void erTextRenderer::drawText(){
    masker.beginLayer();
    {
        ofClear(ofColor(ofColor::white, 0));
        ofSetColor(ofColor::white);
        int i = textY.val();
        for(auto text : *currentTexts){
            ofDrawBitmapString(text, indentX, i+=textHeight);
        }
    }
    masker.endLayer();

    masker.beginMask();
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        ofDrawRectangle(0, 0, 900, numLines * textHeight);
    }
    masker.endMask();

    masker.draw(0, 40);
    if(settings.clientDrawingEnabled){
        masker.drawOverlay();
    }
}