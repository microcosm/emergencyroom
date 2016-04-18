#pragma once
#include "ofMain.h"
#include "ofxCsvStream.h"
#include "ofxShivaVGRenderer.h"
#include "ofxLayerMask.h"
#include "ofxPostProcessing.h"
#include "ofxShapeSystem.h"

#define ECG_DATA_SOURCE "ecg.csv"
#define ECG_NUM_COLS 2

#define ECG_HIGHEST_VALUE -0.177
#define ECG_LOWEST_VALUE -1.78
#define ECG_START_ROW 168
#define ECG_EXIT_ROW 403

#define ECG_MAX_POINTS 200
#define ECG_PERIOD 1600

#define ECG_LINE_TAIL_FROM_LEFT 0.2
#define ECG_LINE_TAIL_TO_RIGHT 0.5

#define ECG_GRID_DIVISIONS_X 16
#define ECG_GRID_DIVISIONS_Y 10

#define ECG_RADIAL_MULTIPLIER 0.85
#define ECG_LINE_HEAD_SIZE 35

class erEcgVisualization{

public:
    void setup();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);

protected:
    void readData();
    void loadNewPoints();
    void trimPointsToSize();
    void createLinearMaskImage();
    void createRadialMaskShape();
    void createLineHeadShape();
    void renderGridLayer();
    void renderEcgLineLayer();
    void renderEcgLineMask();
    void renderRadialOverlayLayer();
    void renderRadialOverlayMask();

    ofxCsvStream stream;
    vector<float> data;
    int numRows, currentRow, lastRow;

    ofxLayerMask masker;
    ofColor lineColor;
    ofImage linearMaskImage;
    ofxShapeSystem radialShapeSystem;
    ofxShape radialShape;
    ofxShapeSystem lineHeadShapeSystem;
    ofxShape lineHeadShape;
    ofPtr<ofxShivaVGRenderer> shivaRenderer;
    ofPtr<ofBaseRenderer> defaultRenderer;
    ofxPostProcessing post;

    float currentValue;
    float timeIndex, incrementalTimeIndex, lastTimeIndex;
    ofPoint point, oldPoint, gridIncrement;
    deque<ofPoint> points;
};