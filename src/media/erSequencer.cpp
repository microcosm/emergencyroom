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
    cout << "current channel " << currentChannel;
    if(!renderer->isChannelPlaying(currentChannel)){
        cout << " not currently playing" << endl;
        params.newVideoCommand();
        params.setPath(chooseVideo());
        params.setSpeed(1);
        network->target(currentChannel, params);
        renderer->preview(currentChannel, params);
        erLog("erSequencer::playNewVideo()", "Target channel " + ofToString(currentChannel) + " " + params.getArgumentStr());
    }else{
        cout << " IS playing" << endl;
    }
    incrementCurrentChannel();
}

string erSequencer::chooseVideo(){
    string path;
    do{
        path = chooseRandom(isAudioPlaying() ? &mediaLoader->silentVideos : &mediaLoader->audibleVideos);
    }while(mediaLoader->videoPlayers[path]->isPlaying());
    return path;
}

string erSequencer::chooseRandom(vector<string>* videos){
    int index = (int)floor(ofRandom(videos->size()));
    index = index == videos->size() ? index - 1 : index;
    return videos->at(index);
}

bool erSequencer::isAudioPlaying(){
    for(auto const& path : mediaLoader->audibleVideos){
        if(mediaLoader->videoPlayers[path]->isOrWillBePlaying()){
            return true;
        }
    }
    return false;
}

int erSequencer::incrementCurrentChannel(){
    currentChannel++;
    if(currentChannel > numChannels){
        currentChannel = 1;
    }
    return currentChannel;
}