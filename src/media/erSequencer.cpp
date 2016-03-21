#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaManager* _mediaManager){
    network = _network;
    mediaManager = _mediaManager;
    currentChannel = 1;
    numChannels = _network->getNumChannels();

    translater = network->getTranslater();
    allVideos = mediaManager->getAllVideos();

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
        mediaManager->play(params);
    }else if(params.isGraphicCommand()){
        mediaManager->render(params);
    }
}

void erSequencer::playNewVideo(){
    params.newVideoCommand();
    params.setPath(chooseVideo());
    params.setSpeed(1);
    network->target(currentChannel, params);
    mediaManager->play(params);
    erLog("erSequencer::playNewVideo()", "Target channel " + ofToString(currentChannel) + " " + params.getArgumentStr());
    incrementCurrentChannel();
}

string erSequencer::chooseVideo(){
    int index = (int)floor(ofRandom(allVideos.size()));
    index = index == allVideos.size() ? index - 1 : index;
    return allVideos.at(index);
}

void erSequencer::incrementCurrentChannel(){
    currentChannel++;
    if(currentChannel > numChannels){
        currentChannel = 1;
    }
}