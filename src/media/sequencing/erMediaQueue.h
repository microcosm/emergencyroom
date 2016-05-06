#pragma once
#include "ofMain.h"
#include "erMediaLoader.h"

class erMediaQueue{

public:
    void setup(erMediaLoader* _loader, string _collection);
    void ensureLoaded();
    string getNextSilent();
    string getNextAudible();

protected:
    void loadQueues();
    erMediaLoader* loader;
    string collection;
    vector<string>* audibleVideosForCollection;
    vector<string>* silentVideosForCollection;
    vector<int> shuffledAudibleVideoIndices, shuffledSilentVideoIndices;
    int currentAudibleVideoIndex, currentSilentVideoIndex;
    bool queuesLoaded;
};