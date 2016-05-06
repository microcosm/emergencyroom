#pragma once
#include "ofMain.h"
#include "erMediaLoader.h"

class erMediaQueue{

public:
    void setup(erMediaLoader* _loader);
    void ensureQueuesLoaded();
    string getNextSilent();
    string getNextAudible();

protected:
    void loadQueues();
    erMediaLoader* loader;
    vector<int> shuffledAudibleVideoIndices, shuffledSilentVideoIndices;
    int currentAudibleVideoIndex, currentSilentVideoIndex;
    bool queuesLoaded;
};