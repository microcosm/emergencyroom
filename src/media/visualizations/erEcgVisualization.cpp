#include "erEcgVisualization.h"

void erEcgVisualization::setup(){
    ofToggleFullscreen();
    masker.setup(1);
    generateMaskImage();

    currentRow = 0;
    gridIncrement.x = ofGetWidth() / 16;
    gridIncrement.y = ofGetHeight() / 10;

    shivaRenderer = ofPtr<ofxShivaVGRenderer>(new ofxShivaVGRenderer);
    defaultRenderer = ofGetCurrentRenderer();
    shivaRenderer->setLineCapStyle(VG_CAP_ROUND);
    post.init();
    post.createPass<BloomPass>()->setEnabled(true);

    readData();

    ofAddListener(ofEvents().update, this, &erEcgVisualization::update);
    ofAddListener(ofEvents().draw, this, &erEcgVisualization::draw);
}

void erEcgVisualization::readData(){
    stream.openReadStream(ECG_DATA_SOURCE);
    numRows = 0;
    int i = 0;
    while(!stream.eof()){
        stream.readNextLine();
        if(i >= ECG_START_ROW && i < ECG_EXIT_ROW){
            for(int j = 0; j < stream.getCurrentTokenSize(); j += ECG_NUM_COLS){
                data.push_back(stream.getValue<float>(j+1));
            }
            numRows++;
        }
        i++;
    }
}

void erEcgVisualization::update(ofEventArgs& args){
    lastRow = currentRow;
    lastTimeIndex = timeIndex;
    timeIndex = ofGetElapsedTimeMillis() % ECG_PERIOD;
    currentRow = ofMap(timeIndex, 0, ECG_PERIOD, 0, numRows-1);

    loadNewPoints();
    trimPointsToSize();
}

void erEcgVisualization::draw(ofEventArgs& args){
    ofBackground(ofColor::black);
    post.begin();
    {
        drawGrid();
        renderEcgLine();
        renderEcgMask();
        masker.draw();
    }
    post.end();
    post.draw();
    masker.drawOverlay();
}

void erEcgVisualization::generateMaskImage(){
    maskImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    for(int x = 0; x < ofGetWidth(); x++){
        for(int y = 0; y < ofGetHeight(); y++){
            float begin = ofGetWidth() * ECG_LINE_TAIL_BEGIN;
            float end = ofGetWidth() * ECG_LINE_TAIL_END;
            float brightness = ofMap(x, begin, end, 0, 255);
            color = ofColor::fromHsb(0, 0, brightness);
            maskImage.setColor(x, y, color);
        }
    }
    maskImage.update();
}

void erEcgVisualization::drawGrid(){
    ofSetLineWidth(1);
    ofSetColor(ofColor::white, 5);
    for(int x = gridIncrement.x; x < ofGetWidth(); x += gridIncrement.x){
        for(int y = gridIncrement.y; y < ofGetHeight(); y += gridIncrement.y){
            ofDrawLine(x, 0, x, ofGetHeight());
            ofDrawLine(0, y, ofGetWidth(), y);
        }
    }
}

void erEcgVisualization::renderEcgLine(){
    masker.beginLayer();
    {
        ofClear(ofColor(ofColor::black, 0));
        ofSetCurrentRenderer(shivaRenderer);
        ofSetLineWidth(7);
        ofSetColor(ofColor::white);
        for(int i = 0; i < points.size(); i++){
            if(i > 0){
                point = points[i];
                oldPoint = points[i-1];
                if(point.x >= oldPoint.x){
                    ofDrawLine(oldPoint, point);
                }
            }
        }
        ofSetCurrentRenderer(defaultRenderer);
    }
    masker.endLayer();
}

void erEcgVisualization::renderEcgMask(){
    masker.beginMask();
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        maskImage.draw(points.back().x, 0);
        maskImage.draw(points.back().x - ofGetWidth(), 0);
    }
    masker.endMask();
}

void erEcgVisualization::loadNewPoints(){
    for(int i = lastRow; i <= currentRow; i++){
        currentValue = data.at(i);
        incrementalTimeIndex = ofMap(i, lastRow, currentRow, lastTimeIndex, timeIndex);
        point.x = ofMap(incrementalTimeIndex, 0, ECG_PERIOD, 0, ofGetWidth());
        point.y = ofMap(currentValue, ECG_HIGHEST_VALUE, ECG_LOWEST_VALUE, 0, ofGetHeight());
        if(i > lastRow){
            points.push_back(point);
        }
    }
}

void erEcgVisualization::trimPointsToSize(){
    if(points.size() > ECG_MAX_POINTS){
        points.erase(points.begin(), points.begin() + points.size() - ECG_MAX_POINTS);
    }
}