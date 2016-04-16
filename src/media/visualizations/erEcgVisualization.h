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
    int numCols, numRows, currentRow, lastRow, startRow, exitRow;
    int period, maxPoints, tailLength;
    float currentValue, highestValue, lowestValue;
    float timeIndex, incrementalTimeIndex, lastTimeIndex;
    ofPoint point, oldPoint;
    deque<ofPoint> points;
    float alpha, alphaIncrement;
};