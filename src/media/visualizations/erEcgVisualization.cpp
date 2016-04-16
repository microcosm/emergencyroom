#include "erEcgVisualization.h"

void erEcgVisualization::setup(){
    source = "ecg.csv";
    numCols = 2;
    highestValue = -0.177;
    lowestValue = -1.78;
    startRow = 168;
    exitRow = 403;
    size = 5;
    period = 1000;
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
    timeIndex = ofGetElapsedTimeMillis() % period;
    currentRow = ofMap(timeIndex, 0, period, 0, numRows-1);
    currentValue = data.at(currentRow);
}

void erEcgVisualization::draw(ofEventArgs& args){
    x = ofMap(timeIndex, 0, period, 0, ofGetWidth() - size);
    y = ofMap(currentValue, highestValue, lowestValue, 0, ofGetHeight() - size);
    ofSetColor(ofColor::white);
    ofDrawRectangle(x, y, size, size);
}