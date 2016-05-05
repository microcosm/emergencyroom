#include "erTextRenderer.h"

void erTextRenderer::setup(){
    ofAddListener(ofEvents().draw, this, &erTextRenderer::draw);
}

void erTextRenderer::draw(ofEventArgs& args){
    if(currentVideoPlayer != NULL && currentVideoPlayer->isPlaying()){
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

void erTextRenderer::execute(erPlayParams params, ofPtr<erSyncedVideoPlayer> _videoPlayer){
    currentTexts = &texts->at(params.getPath());
    currentVideoPlayer = _videoPlayer;
}