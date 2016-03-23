#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaLoader* _mediaLoader, erMediaRenderer* _renderer){
    network = _network;
    mediaLoader = _mediaLoader;
    renderer = _renderer;

    currentChannel = 1;
    numChannels = network->getNumChannels();
    translater = network->getTranslater();

    ofAddListener(ofEvents().update, this, &erSequencer::update);
    ofAddListener(network->clientMessageReceived(), this, &erSequencer::messageReceived);
}

void erSequencer::update(ofEventArgs& updateArgs){
    if(network->isRunningServer() && ofGetFrameNum() % ER_VIDEO_LENGTH == 0){
        playNewVideo();
    }
}

void erSequencer::messageReceived(string& message){
    erLog("erSequencer::messageReceived(string& message)", message);
    params = translater->toParams(message);
    if(params.isPlayable()){
        renderer->play(params);
    }
}

void erSequencer::playNewVideo(){
    params.newVideoCommand();
    params.setPath(chooseVideo());
    params.setSpeed(1);
    network->target(currentChannel, params);
    renderer->preview(currentChannel, params);
    erLog("erSequencer::playNewVideo()", "Target channel " + ofToString(currentChannel) + " " + params.getArgumentStr());
    incrementCurrentChannel();
}

string erSequencer::chooseVideo(){
    int index = (int)floor(ofRandom(mediaLoader->allVideos.size()));
    index = index == mediaLoader->allVideos.size() ? index - 1 : index;
    return mediaLoader->allVideos.at(index);
}

void erSequencer::incrementCurrentChannel(){
    currentChannel++;
    if(currentChannel > numChannels){
        currentChannel = 1;
    }
}