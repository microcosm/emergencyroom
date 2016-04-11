#include "erMediaPlayer.h"

void erMediaPlayer::setup(erNetwork* _network, int _numChannels){
    network = _network;
    numChannels = _numChannels;

    calculatePreviewSize();
    ofAddListener(ofEvents().update, this, &erMediaPlayer::update);
    ofAddListener(ofEvents().draw, this, &erMediaPlayer::draw);
}

void erMediaPlayer::update(ofEventArgs& args){
    testVideoPlayer->update();
    for(auto const& player : *videoPlayers){
        player.second->update();
    }

    eraseCompletedVideosFromChannels();
}

void erMediaPlayer::draw(ofEventArgs& args){
    if(network->isRunningServer()){
        drawServer();
    }else{
        drawClient();
    }
}

void erMediaPlayer::play(erPlayParams params){
    if(params.isVideoCommand()){
        videoPlayers->at(params.getPath())->execute(params);
    }else if(params.isTestCommand()){
        testSoundPlayer->execute(params);
        testVideoPlayer->execute(params);
    }
}

void erMediaPlayer::preview(int channel, erPlayParams params){
    play(params);
    channelsToPlayers[channel] = videoPlayers->at(params.getPath());
}

bool erMediaPlayer::isChannelPlaying(int channel){
    return hasChannel(channel) && channelsToPlayers[channel].get()->isOrWillBePlaying();
}

void erMediaPlayer::setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer){
    testSoundPlayer = _testSoundPlayer;
}

void erMediaPlayer::setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer){
    testVideoPlayer = _testVideoPlayer;
}

void erMediaPlayer::setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
}

void erMediaPlayer::calculatePreviewSize(){
    previewWidth = (ofGetWidth() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
    previewHeight = (ofGetHeight() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
}

bool erMediaPlayer::hasChannel(int channel){
    return channelsToPlayers.count(channel) == 1;
}

void erMediaPlayer::eraseCompletedVideosFromChannels(){
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

void erMediaPlayer::drawClient(){
    ofClear(ofColor::black);
    renderer.drawVideo(testVideoPlayer, 0, 0, ofGetWidth(), ofGetHeight());
    for(auto const& player : *videoPlayers){
        renderer.drawVideo(player.second.get(), 0, 0, ofGetWidth(), ofGetHeight());
    }
}

void erMediaPlayer::drawServer(){
    ofClear(ofColor::black);
    
    ofSetColor(ofColor::white);
    ofNoFill();

    currentChannel = 1;
    for(int xi = 0; xi < 3; xi++){
        for(int yi = 0; yi < 3; yi++){
            x = getX(xi);
            y = getY(yi);
            if(hasChannel(currentChannel)) {
                renderer.drawVideo(channelsToPlayers[currentChannel].get(), x, y, previewWidth, previewHeight);
            }
            drawPreviewBorder(x, y, currentChannel);
            currentChannel++;
        }
    }

    renderer.drawVideo(testVideoPlayer, SCREEN_MARGIN, SCREEN_MARGIN, ofGetWidth() - DOUBLE_SCREEN_MARGIN, ofGetHeight() - DOUBLE_SCREEN_MARGIN);
}

void erMediaPlayer::drawPreviewBorder(int x, int y, int channel){
    ofDrawRectangle(x, y, previewWidth, previewHeight);
    currentChannelStr = "CHANNEL " + ofToString(channel);
    ofDrawBitmapString(currentChannelStr, x + SCREEN_MARGIN, y + DOUBLE_SCREEN_MARGIN);
}

int erMediaPlayer::getX(int xi){
    return SCREEN_MARGIN * (xi + 1) + previewWidth * xi;
}

int erMediaPlayer::getY(int yi){
    return SCREEN_MARGIN * (yi + 1) + previewHeight * yi;
}