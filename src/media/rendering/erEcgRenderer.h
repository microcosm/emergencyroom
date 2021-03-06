#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "ofxCsvStream.h"
#include "ofxLayerMask.h"
#include "ofxShapeSystem.h"
#include "erThreadedPlayer.h"
#include "erNetwork.h"
#include "erEcgTimer.h"

#ifdef __APPLE__
#include "ofxShivaVGRenderer.h"
#include "ofxPostProcessing.h"
#endif

#define ECG_DATA_SOURCE "ecg.csv"
#define ECG_NUM_COLS 2

#define ECG_HIGHEST_VALUE -0.177
#define ECG_LOWEST_VALUE -1.78
#define ECG_START_ROW 168
#define ECG_EXIT_ROW 403

#define ECG_MAX_POINTS 200

#define ECG_LINE_TAIL_FROM_LEFT 0.2
#define ECG_LINE_TAIL_TO_RIGHT 0.5

#define ECG_GRID_DIVISIONS_X 16
#define ECG_GRID_DIVISIONS_Y 10

#define ECG_RADIAL_MULTIPLIER 0.85
#define ECG_LINE_HEAD_SIZE 35

#define ECG_SYNC_DELAY 3000

class erEcgRenderer{

public:
    void setup(erEcgTimer* _ecgTimer);
    void update();
    void draw();

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

    int width, height;

#ifdef __APPLE__
    ofPtr<ofxShivaVGRenderer> shivaRenderer;
    ofPtr<ofBaseRenderer> defaultRenderer;
    ofxPostProcessing post;
#endif

    float periodPosition, nthPeriodPosition, lastPeriodPosition;//?
    ofPoint point, oldPoint, gridIncrement;
    deque<ofPoint> points;

    erNetwork* network;
    u_int64_t timeOffset;

    bool isPlaying;
    erEcgTimer* ecgTimer;
};
