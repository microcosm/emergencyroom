#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player){
    network = _network;
    loader = _loader;
    player = _player;

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
        player->play(params);
    }
}

void erSequencer::playNewVideo(){
    if(!player->isChannelPlaying(currentChannel)){
        params.newVideoCommand();
        params.setPath(chooseVideo());
        params.setSpeed(1);
        network->target(currentChannel, params);
        player->preview(currentChannel, params);
        erLog("erSequencer::playNewVideo()", "Target channel " + ofToString(currentChannel) + " " + params.getArgumentStr());
    }
    incrementCurrentChannel();
}

string erSequencer::chooseVideo(){
    string path;
    do{
        path = chooseRandom(isAudioPlaying() ? &loader->silentVideos : &loader->audibleVideos);
    }while(loader->videoPlayers[path]->isOrWillBePlaying());
    return path;
}

string erSequencer::chooseRandom(vector<string>* videos){
    int index = (int)floor(ofRandom(videos->size()));
    index = index == videos->size() ? index - 1 : index;
    return videos->at(index);
}

bool erSequencer::isAudioPlaying(){
    for(auto const& path : loader->audibleVideos){
        if(loader->videoPlayers[path]->isOrWillBePlaying()){
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