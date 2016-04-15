#pragma once
#include "ofMain.h"
#include "ofxCsvStream.h"

class erEcgVisualization{

public:
    void setup();

protected:
    void readData();
    ofxCsvStream stream;
    map<float,float> data;
    string source;
    int numCols;
};