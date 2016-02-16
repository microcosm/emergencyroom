#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaManager* _mediaManager){
    network = _network;
    mediaManager = _mediaManager;

    translater = network->getTranslater();
    videoCollections = mediaManager->getVideoCollections();
    channelsToCollections.resize(network->getNumChannels());
    numCollections = videoCollections.size();

    ofAddListener(ofEvents().update, this, &erSequencer::update);
    ofAddListener(network->clientMessageReceived(), this, &erSequencer::messageReceived);
}

void erSequencer::update(ofEventArgs& updateArgs){
    if(ofGetFrameNum() % ER_COLLECTION_LENGTH == 0){
        cout << ofGetFrameNum() << ": ";
        for(int i = 0; i < channelsToCollections.size(); i++){
            channelsToCollections[i] = selectCollection();
            cout << channelsToCollections[i] << " ";
        }
        cout << endl;
    }
}

void erSequencer::messageReceived(string& message){
    params = translater->toParams(message);
    if(params.isPlayable()){
        mediaManager->play(params);
    }
}

string erSequencer::selectCollection(){
    return videoCollections[floor(ofRandom(numCollections))];
}