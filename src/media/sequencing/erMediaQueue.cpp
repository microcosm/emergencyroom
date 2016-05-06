#include "erMediaQueue.h"

void erMediaQueue::setup(erMediaLoader* _loader, string _collection){
    loader = _loader;
    collection = _collection;
    queuesLoaded = false;
    currentAudibleVideoIndex = -1;
    currentSilentVideoIndex = -1;
}

void erMediaQueue::ensureLoaded(){
    if(!queuesLoaded){
        loadQueues();
    }
}

string erMediaQueue::getNextSilent(){
    if(currentSilentVideoIndex < 0 || currentSilentVideoIndex >= silentVideosForCollection->size()){
        currentSilentVideoIndex = 0;
        random_shuffle(shuffledSilentVideoIndices.begin(), shuffledSilentVideoIndices.end());
    }
    return silentVideosForCollection->at(shuffledSilentVideoIndices.at(currentSilentVideoIndex++));
}

string erMediaQueue::getNextAudible(){
    if(currentAudibleVideoIndex < 0 || currentAudibleVideoIndex >= audibleVideosForCollection->size()){
        currentAudibleVideoIndex = 0;
        random_shuffle(shuffledAudibleVideoIndices.begin(), shuffledAudibleVideoIndices.end());
    }
    return audibleVideosForCollection->at(shuffledAudibleVideoIndices.at(currentAudibleVideoIndex++));
}

void erMediaQueue::loadQueues(){
    audibleVideosForCollection = &loader->collectionsToAudibleVideos[collection];
    silentVideosForCollection = &loader->collectionsToSilentVideos[collection];
    shuffledAudibleVideoIndices.clear();
    shuffledSilentVideoIndices.clear();

    for(int i = 0; i < audibleVideosForCollection->size(); i++){
        shuffledAudibleVideoIndices.push_back(i);
    }

    for(int i = 0; i < silentVideosForCollection->size(); i++){
        shuffledSilentVideoIndices.push_back(i);
    }

    queuesLoaded = true;
}