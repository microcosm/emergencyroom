#pragma once
#include "ofMain.h"
#include "ofxCsvStream.h"

class erEcgVisualization{

public:
    void setup();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);

protected:
    void readData();
    ofxCsvStream stream;
    vector<float> data;
    string source;
    int numCols, numRows, currentRow;
    float currentValue, highestValue, lowestValue;
    int y, startRow, exitRow;
};