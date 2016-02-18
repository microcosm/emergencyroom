#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaManager* _mediaManager){
    network = _network;
    mediaManager = _mediaManager;
    serverJustStarted = true;
    currentChannel = 0;

    translater = network->getTranslater();
    videoCollections = mediaManager->getVideoCollections();
    channelsToCollections.resize(network->getNumChannels());
    collectionsToVideos = mediaManager->getCollectionsToVideos();
    numCollections = videoCollections.size();

    ofAddListener(ofEvents().update, this, &erSequencer::update);
    ofAddListener(network->clientMessageReceived(), this, &erSequencer::messageReceived);
}

void erSequencer::update(ofEventArgs& updateArgs){
    if(network->isRunningServer()){
        if(serverJustStarted){
            assignCollectionsToChannels();
        }
        runServerTasks();
        serverJustStarted = false;
    }else{
        serverJustStarted = true;
    }
}

void erSequencer::messageReceived(string& message){
    erLog("erSequencer::messageReceived(string&message)", message);
    params = translater->toParams(message);
    if(params.isPlayable()){
        mediaManager->play(params);
    }
}

void erSequencer::runServerTasks(){
    if(ofGetFrameNum() % ER_COLLECTION_LENGTH == 0){
        assignCollectionsToChannels();
    }
    if(ofGetFrameNum() % ER_VIDEO_LENGTH == 0){
        playNewVideos();
    }
}

void erSequencer::playNewVideos(){
    params.newPlayCommand();
    params.setPath(chooseVideo(currentChannel));
    params.setSpeed(ofRandom(0.75, 0.25));
    network->target(currentChannel, params);
    mediaManager->play(params);
    incrementCurrentChannel();
    erLog("erSequencer::playNewVideos()", params.getArgumentStr());
}

void erSequencer::assignCollectionsToChannels(){
    for(int i = 0; i < channelsToCollections.size(); i++){
        channelsToCollections[i] = selectCollection();
    }
}

string erSequencer::selectCollection(){
    return videoCollections[floor(ofRandom(numCollections))];
}

void erSequencer::incrementCurrentChannel(){
    currentChannel++;
    if(currentChannel >= channelsToCollections.size()){
        currentChannel = 0;
    }
}

string erSequencer::chooseVideo(int currentChannel){
    videos = &collectionsToVideos[channelsToCollections[currentChannel]];
    return videos->at((int)ofRandom(videos->size()));
}