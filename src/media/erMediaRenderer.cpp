#include "erMediaRenderer.h"

void erMediaRenderer::setup(erNetwork* _network, int _numChannels){
    network = _network;
    numChannels = _numChannels;

    fbo.allocate(ofGetWidth(), ofGetHeight());
    fboGlitch.allocate(ofGetWidth(), ofGetHeight());
    calculatePreviewSize();
    ofAddListener(ofEvents().update, this, &erMediaRenderer::update);
    ofAddListener(ofEvents().draw, this, &erMediaRenderer::draw);
}

void erMediaRenderer::update(ofEventArgs& args){
    testVideoPlayer->update();
    for(auto const& player : *videoPlayers){
        player.second->update();
    }

    toErase.clear();
    for(auto const& channelToPlayer : channelsToPlayers){
        if(!channelToPlayer.second.get()->isOrWillBePlaying()){
            toErase.push_back(channelToPlayer.first);
        }
    }

    for(auto const& i : toErase){
        channelsToPlayers.erase(channelsToPlayers.find(i));
    }
}

void erMediaRenderer::draw(ofEventArgs& args){
    if(network->isRunningServer()){
        drawServer();
    }else{
        drawClient();
    }
}

void erMediaRenderer::play(erPlayParams params){
    if(params.isVideoCommand()){
        videoPlayers->at(params.getPath())->execute(params);
    }else if(params.isTestCommand()){
        testSoundPlayer->execute(params);
        testVideoPlayer->execute(params);
    }
}

void erMediaRenderer::preview(int channel, erPlayParams params){
    play(params);
    channelsToPlayers[channel] = videoPlayers->at(params.getPath());
}

bool erMediaRenderer::isChannelPlaying(int channel){
    return hasChannel(channel) && channelsToPlayers[channel].get()->isOrWillBePlaying();
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

void erMediaRenderer::calculatePreviewSize(){
    previewWidth = (ofGetWidth() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
    previewHeight = (ofGetHeight() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
}

bool erMediaRenderer::hasChannel(int channel){
    return channelsToPlayers.count(channel) == 1;
}

void erMediaRenderer::drawClient(){
    ofClear(ofColor::black);
    drawVideo(testVideoPlayer, 0, 0, ofGetWidth(), ofGetHeight());
    for(auto const& player : *videoPlayers){
        drawGlitched(player.second.get(), 0, 0, ofGetWidth(), ofGetHeight());
    }
}

void erMediaRenderer::drawServer(){
    ofClear(ofColor::black);
    
    ofSetColor(ofColor::white);
    ofNoFill();

    currentChannel = 1;
    for(int xi = 0; xi < 3; xi++){
        for(int yi = 0; yi < 3; yi++){
            x = getX(xi);
            y = getY(yi);
            if(hasChannel(currentChannel)) {
                drawVideo(channelsToPlayers[currentChannel].get(), x, y, previewWidth, previewHeight);
            }
            drawPreviewBorder(x, y, currentChannel);
            currentChannel++;
        }
    }

    drawVideo(testVideoPlayer, SCREEN_MARGIN, SCREEN_MARGIN, ofGetWidth() - DOUBLE_SCREEN_MARGIN, ofGetHeight() - DOUBLE_SCREEN_MARGIN);
}

void erMediaRenderer::drawVideo(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    if(player->isPlaying()){
        player->draw(x, y, width, height);
    }
}

void erMediaRenderer::drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    if(player->isPlaying()){
        fbo.begin();
        {
            player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
        }
        fbo.end();

        fboGlitch.draw(fbo, x, y, ofGetWidth(), ofGetHeight());
    }
}

void erMediaRenderer::drawPreviewBorder(int x, int y, int channel){
    ofDrawRectangle(x, y, previewWidth, previewHeight);
    currentChannelStr = "CHANNEL " + ofToString(channel);
    ofDrawBitmapString(currentChannelStr, x + SCREEN_MARGIN, y + SCREEN_MARGIN + SCREEN_MARGIN);
}

int erMediaRenderer::getX(int xi){
    return SCREEN_MARGIN * (xi + 1) + previewWidth * xi;
}

int erMediaRenderer::getY(int yi){
    return SCREEN_MARGIN * (yi + 1) + previewHeight * yi;
}