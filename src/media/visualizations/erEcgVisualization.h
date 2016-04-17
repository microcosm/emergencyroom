#pragma once
#include "ofMain.h"
#include "ofxCsvStream.h"
#include "ofxShivaVGRenderer.h"

class erEcgVisualization{

public:
    void setup();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);

protected:
    void readData();
    void loadNewPoints();
    void trimPointsToSize();
    void drawGrid();
    void drawEcgLine();

    ofxCsvStream stream;
    vector<float> data;
    string source;
    int numCols, numRows, currentRow, lastRow, startRow, exitRow;
    int period, maxPoints, tailLength;
    float currentValue, highestValue, lowestValue;
    float timeIndex, incrementalTimeIndex, lastTimeIndex;
    ofPoint point, oldPoint, gridIncrement;
    deque<ofPoint> points;
    float alpha, alphaIncrement;

    ofPtr<ofxShivaVGRenderer> shivaRenderer;
};