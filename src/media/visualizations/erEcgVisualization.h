#pragma once
#include "ofMain.h"
#include "ofxCsvStream.h"
#include "ofxShivaVGRenderer.h"

#define ECG_DATA_SOURCE "ecg.csv"
#define ECG_NUM_COLS 2
#define ECG_HIGHEST_VALUE -0.177
#define ECG_LOWEST_VALUE -1.78
#define ECG_START_ROW 168
#define ECG_EXIT_ROW 403
#define ECG_PERIOD 1000

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
    int numRows, currentRow, lastRow;
    int maxPoints, tailLength;
    float currentValue;
    float timeIndex, incrementalTimeIndex, lastTimeIndex;
    ofPoint point, oldPoint, gridIncrement;
    deque<ofPoint> points;
    float alpha, alphaIncrement;

    ofPtr<ofxShivaVGRenderer> shivaRenderer;
};