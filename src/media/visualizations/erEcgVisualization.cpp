#include "erEcgVisualization.h"

void erEcgVisualization::setup(){
    ofToggleFullscreen();

    source = "ecg.csv";
    numCols = 2;
    highestValue = -0.177;
    lowestValue = -1.78;
    startRow = 168;
    exitRow = 403;
    period = 1400;
    currentRow = 0;
    maxPoints = 200;
    tailLength = 140;
    alphaIncrement = 255 / tailLength;
    gridIncrement.x = ofGetWidth() / 16;
    gridIncrement.y = ofGetHeight() / 10;

    shivaRenderer = ofPtr<ofxShivaVGRenderer>(new ofxShivaVGRenderer);
    ofSetCurrentRenderer(shivaRenderer);
    shivaRenderer->setLineCapStyle(VG_CAP_ROUND);
    readData();

    ofAddListener(ofEvents().update, this, &erEcgVisualization::update);
    ofAddListener(ofEvents().draw, this, &erEcgVisualization::draw);
}

void erEcgVisualization::readData(){
    stream.openReadStream(source);
    numRows = 0;
    int i = 0;
    while(!stream.eof()){
        stream.readNextLine();
        if(i >= startRow && i < exitRow){
            for(int j = 0; j < stream.getCurrentTokenSize(); j += numCols){
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
    timeIndex = ofGetElapsedTimeMillis() % period;
    currentRow = ofMap(timeIndex, 0, period, 0, numRows-1);

    loadNewPoints();
    trimPointsToSize();
}

void erEcgVisualization::draw(ofEventArgs& args){
    ofBackground(ofColor::black);
    drawGrid();
    drawEcgLine();
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

void erEcgVisualization::drawEcgLine(){
    ofSetLineWidth(7);
    ofSetColor(ofColor::white);
    alpha = 0;
    for(int i = 0; i < points.size(); i++){
        if(i > 0){
            alpha += alphaIncrement;
            ofSetColor(ofColor::white, alpha);
            
            point = points[i];
            oldPoint = points[i-1];
            if(point.x >= oldPoint.x){
                ofDrawLine(oldPoint, point);
            }
        }
    }
}

void erEcgVisualization::loadNewPoints(){
    for(int i = lastRow; i <= currentRow; i++){
        currentValue = data.at(i);
        incrementalTimeIndex = ofMap(i, lastRow, currentRow, lastTimeIndex, timeIndex);
        point.x = ofMap(incrementalTimeIndex, 0, period, 0, ofGetWidth());
        point.y = ofMap(currentValue, highestValue, lowestValue, 0, ofGetHeight());
        if(i > lastRow){
            points.push_back(point);
        }
    }
}

void erEcgVisualization::trimPointsToSize(){
    if(points.size() > maxPoints){
        points.erase(points.begin(), points.begin() + points.size() - maxPoints);
    }
}