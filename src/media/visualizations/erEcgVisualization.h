#pragma once
#include "ofMain.h"
#include "ofxCsvStream.h"
#include "ofxShivaVGRenderer.h"
#include "ofxLayerMask.h"
#include "ofxPostProcessing.h"

#define ECG_DATA_SOURCE "ecg.csv"
#define ECG_NUM_COLS 2
#define ECG_HIGHEST_VALUE -0.177
#define ECG_LOWEST_VALUE -1.78
#define ECG_START_ROW 168
#define ECG_EXIT_ROW 403
#define ECG_MAX_POINTS 200
#define ECG_PERIOD 1000
#define ECG_LINE_TAIL_BEGIN 0.2
#define ECG_LINE_TAIL_END 0.8

class erEcgVisualization{

public:
    void setup();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);

protected:
    void readData();
    void loadNewPoints();
    void trimPointsToSize();
    void generateMaskImage();
    void drawGrid();
    void renderEcgLine();
    void renderEcgMask();

    ofxCsvStream stream;
    vector<float> data;
    int numRows, currentRow, lastRow;

    ofImage maskImage;
    ofColor color;
    ofxLayerMask masker;
    ofPtr<ofxShivaVGRenderer> shivaRenderer;
    ofPtr<ofBaseRenderer> defaultRenderer;
    ofxPostProcessing post;

    float currentValue;
    float timeIndex, incrementalTimeIndex, lastTimeIndex;
    ofPoint point, oldPoint, gridIncrement;
    deque<ofPoint> points;
};