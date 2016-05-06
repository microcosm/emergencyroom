#include "erMediaQueue.h"

void erMediaQueue::setup(erMediaLoader* _loader){
    loader = _loader;
    queuesLoaded = false;
    currentAudibleVideoIndex = -1;
    currentSilentVideoIndex = -1;
}

void erMediaQueue::ensureQueuesLoaded(){
    if(!queuesLoaded){
        loadQueues();
    }
}

string erMediaQueue::getNextSilent(){
    if(currentSilentVideoIndex < 0 || currentSilentVideoIndex >= loader->silentVideos.size()){
        currentSilentVideoIndex = 0;
        random_shuffle(shuffledSilentVideoIndices.begin(), shuffledSilentVideoIndices.end());
    }
    return loader->silentVideos.at(shuffledSilentVideoIndices.at(currentSilentVideoIndex++));
}

string erMediaQueue::getNextAudible(){
    if(currentAudibleVideoIndex < 0 || currentAudibleVideoIndex >= loader->audibleVideos.size()){
        currentAudibleVideoIndex = 0;
        random_shuffle(shuffledAudibleVideoIndices.begin(), shuffledAudibleVideoIndices.end());
    }
    return loader->audibleVideos.at(shuffledAudibleVideoIndices.at(currentAudibleVideoIndex++));
}

void erMediaQueue::loadQueues(){
    shuffledAudibleVideoIndices.clear();
    for(int i = 0; i < loader->audibleVideos.size(); i++){
        shuffledAudibleVideoIndices.push_back(i);
    }
    
    shuffledSilentVideoIndices.clear();
    for(int i = 0; i < loader->silentVideos.size(); i++){
        shuffledSilentVideoIndices.push_back(i);
    }
    
    queuesLoaded = true;
}