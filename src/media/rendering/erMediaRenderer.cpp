#include "erMediaRenderer.h"

void erMediaRenderer::setup(){
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fboGlitch.allocate(ofGetWidth(), ofGetHeight());
    openingGlitchStart = openingGlitchEnd = closingGlitchStart = closingGlitchEnd = 0;
    ofAddListener(ofEvents().update, this, &erMediaRenderer::update);
}

void erMediaRenderer::update(ofEventArgs& args){
    testVideoPlayer->update();
    for(auto const& player : *videoPlayers){
        player.second->update();
    }
}

void erMediaRenderer::setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer){
    testSoundPlayer = _testSoundPlayer;
}

void erMediaRenderer::setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer){
    testVideoPlayer = _testVideoPlayer;
}

void erMediaRenderer::setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
}

void erMediaRenderer::newOpeningGlitchPeriod(unsigned long long from, float duration){
    openingGlitchStart = from;
    openingGlitchEnd = openingGlitchStart + duration;
}

void erMediaRenderer::newClosingGlitchPeriod(unsigned long long from, float duration){
    closingGlitchStart = from;
    closingGlitchEnd = closingGlitchStart + duration;
}

void erMediaRenderer::draw(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    if(player->isPlaying()){
        withinGlitchPeriod() ? drawGlitched(player, x, y, width, height) : drawNormal(player, x, y, width, height);
    }
}

void erMediaRenderer::drawStatic(){
    ofSetColor(ofColor::white);
    ofDrawBitmapString("STATIC NOW", 10, 10);
}

void erMediaRenderer::drawNormal(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    player->draw(x, y, width, height);
}

void erMediaRenderer::drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    fbo.begin();
    {
        player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
    }
    fbo.end();
    fboGlitch.draw(fbo, x, y, ofGetWidth(), ofGetHeight());
}

bool erMediaRenderer::withinGlitchPeriod(){
    currentTime = ofGetElapsedTimeMillis();
    withinOpeningGlitchPeriod = currentTime > openingGlitchStart && currentTime < openingGlitchEnd;
    withinClosingGlitchPeriod = currentTime > closingGlitchStart && currentTime < closingGlitchEnd;
    return withinOpeningGlitchPeriod || withinClosingGlitchPeriod;
}