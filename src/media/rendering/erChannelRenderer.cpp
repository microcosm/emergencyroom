#include "erChannelRenderer.h"

void erChannelRenderer::setup(erNetwork* _network){
    network = _network;
    mediaRenderer.setup(network);
    calculatePreviewSize();
    ofAddListener(ofEvents().update, this, &erChannelRenderer::update);
    ofAddListener(ofEvents().draw, this, &erChannelRenderer::draw);
}

void erChannelRenderer::update(ofEventArgs& args){
    eraseCompletedVideosFromChannels();
}

void erChannelRenderer::draw(ofEventArgs& args){
    if(network->isRunningServer()){
        drawServer();
    }

    if(network->isRunningClient()){
        drawClient();
    }
}

void erChannelRenderer::setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer){
    testSoundPlayer = _testSoundPlayer;
    mediaRenderer.setTestSoundPlayer(testSoundPlayer);
}

void erChannelRenderer::setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer){
    testVideoPlayer = _testVideoPlayer;
    mediaRenderer.setTestVideoPlayer(testVideoPlayer);
}

void erChannelRenderer::setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
    mediaRenderer.setVideoPlayers(videoPlayers);
}

void erChannelRenderer::newOpeningGlitchPeriod(u_int64_t from, float duration, int channel){
    mediaRenderer.newOpeningGlitchPeriod(from, duration, channel);
}

void erChannelRenderer::newClosingGlitchPeriod(u_int64_t from, float duration, int channel){
    mediaRenderer.newClosingGlitchPeriod(from, duration, channel);
}

void erChannelRenderer::newIntermediateGlitchPeriod(int i, u_int64_t from, float duration, int channel){
    mediaRenderer.newIntermediateGlitchPeriod(i, from, duration, channel);
}

bool erChannelRenderer::isChannelPlaying(int channel){
    return hasChannel(channel) && channelsToPlayers[channel].get()->isOrWillBePlaying();
}

void erChannelRenderer::assign(int channel, erPlayParams params){
    channelsToPlayers[channel] = videoPlayers->at(params.getPath());
}

void erChannelRenderer::calculatePreviewSize(){
    previewWidth = (ofGetWidth() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
    previewHeight = (ofGetHeight() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
}

bool erChannelRenderer::hasChannel(int channel){
    return channelsToPlayers.count(channel) == 1;
}

void erChannelRenderer::eraseCompletedVideosFromChannels(){
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

void erChannelRenderer::drawClient(){
    ofClear(ofColor::black);
    anyPlayerIsPlaying = false;
    for(auto const& player : *videoPlayers){
        videoPlayer = player.second.get();
        if(videoPlayer->isPlaying()){
            anyPlayerIsPlaying = true;
            mediaRenderer.draw(videoPlayer, 0, 0, ofGetWidth(), ofGetHeight());
        }
    }
    if(!anyPlayerIsPlaying){
        mediaRenderer.drawStatic(0, 0, ofGetWidth(), ofGetHeight());
    }
    mediaRenderer.draw(testVideoPlayer, 0, 0, ofGetWidth(), ofGetHeight());
}

void erChannelRenderer::drawServer(){
    ofClear(ofColor::black);

    ofSetColor(ofColor::white);
    ofNoFill();

    currentChannel = 1;
    for(int xi = 0; xi < 3; xi++){
        for(int yi = 0; yi < 3; yi++){
            x = getX(xi);
            y = getY(yi);
            if(hasChannel(currentChannel)) {
                mediaRenderer.draw(channelsToPlayers[currentChannel].get(), x, y, previewWidth, previewHeight, currentChannel);
                if(mediaRenderer.withinGlitchPeriod(currentChannel)){
                    ofSetColor(ofColor::red, 80);
                    ofFill();
                    ofDrawRectangle(x, y, previewWidth, previewHeight);
                }
            }
            drawPreviewBorder(x, y, currentChannel);
            currentChannel++;
        }
    }
    mediaRenderer.draw(testVideoPlayer, SCREEN_MARGIN, SCREEN_MARGIN, ofGetWidth() - DOUBLE_SCREEN_MARGIN, ofGetHeight() - DOUBLE_SCREEN_MARGIN);

    if(settings.serverDrawingEnabled){
        ofFill();
        ofSetColor(ofColor::black, 100);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
}

void erChannelRenderer::drawPreviewBorder(int x, int y, int channel){
    ofSetColor(ofColor::white);
    ofNoFill();
    ofDrawRectangle(x, y, previewWidth, previewHeight);
    currentChannelStr = "CHANNEL " + ofToString(channel);
    ofDrawBitmapString(currentChannelStr, x + DOUBLE_SCREEN_MARGIN, y + previewHeight - DOUBLE_SCREEN_MARGIN);
}

int erChannelRenderer::getX(int xi){
    return SCREEN_MARGIN * (xi + 1) + previewWidth * xi;
}

int erChannelRenderer::getY(int yi){
    return SCREEN_MARGIN * (yi + 1) + previewHeight * yi;
}