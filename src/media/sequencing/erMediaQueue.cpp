#include "erMediaQueue.h"

void erMediaQueue::setup(erMediaLoader* _mediaLoader, string _collection){
    mediaLoader = _mediaLoader;
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
        erShuffle(shuffledSilentVideoIndices);
    }
    return silentVideosForCollection->at(shuffledSilentVideoIndices.at(currentSilentVideoIndex++));
}

string erMediaQueue::getNextAudible(){
    if(currentAudibleVideoIndex < 0 || currentAudibleVideoIndex >= audibleVideosForCollection->size()){
        currentAudibleVideoIndex = 0;
        erShuffle(shuffledAudibleVideoIndices);
    }
    return audibleVideosForCollection->at(shuffledAudibleVideoIndices.at(currentAudibleVideoIndex++));
}

void erMediaQueue::loadQueues(){
    audibleVideosForCollection = &mediaLoader->collectionsToAudibleVideos[collection];
    silentVideosForCollection = &mediaLoader->collectionsToSilentVideos[collection];
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
