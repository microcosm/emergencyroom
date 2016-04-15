#include "erEcgVisualization.h"

void erEcgVisualization::setup(){
    source = "ecg.csv";
    numCols = 2;
    highestValue = -0.177;
    lowestValue = -1.78;
    startRow = 18;
    exitRow = 253;
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
    currentRow = ofMap(ofGetElapsedTimeMillis() % 1000, 0, 1000, 0, numRows-1);
    currentValue = data.at(currentRow);
    y = ofMap(currentValue, lowestValue, highestValue, 0, ofGetHeight());
}

void erEcgVisualization::draw(ofEventArgs& args){
    ofSetColor(ofColor::white);
    ofDrawRectangle(5, y, 5, 5);
}