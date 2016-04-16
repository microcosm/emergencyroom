#include "erEcgVisualization.h"

void erEcgVisualization::setup(){
    source = "ecg.csv";
    numCols = 2;
    highestValue = -0.177;
    lowestValue = -1.78;
    startRow = 168;
    exitRow = 403;
    period = 1000;
    currentRow = 0;
    maxPoints = 80;
    stream.openReadStream(source);
    readData();
    ofAddListener(ofEvents().update, this, &erEcgVisualization::update);
    ofAddListener(ofEvents().draw, this, &erEcgVisualization::draw);
}

void erEcgVisualization::readData(){
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

    for(int i = lastRow + 1; i <= currentRow; i++){
        currentValue = data.at(i);
        incrementalTimeIndex = ofMap(i, lastRow + 1, currentRow, lastTimeIndex, timeIndex);
        point.x = ofMap(incrementalTimeIndex, 0, period, 0, ofGetWidth());
        point.y = ofMap(currentValue, highestValue, lowestValue, 0, ofGetHeight());
        points.push_back(point);
    }
    if(points.size() > maxPoints){
        points.erase(points.begin(), points.begin() + points.size() - maxPoints);
    }
}

void erEcgVisualization::draw(ofEventArgs& args){
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
}