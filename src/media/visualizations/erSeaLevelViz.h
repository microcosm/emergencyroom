#pragma once
#include "ofMain.h"
#include "ofxCsvStream.h"

class erSeaLevelViz{

public:
    void setup();
    
protected:
    void readData();
    ofxCsvStream stream;
    map<int,float> data;
    string source;
    int numCols;
};