#include "erEcgVisualization.h"

void erEcgVisualization::setup(){
    ofToggleFullscreen();
    masker.setup(3);
    //masker.toggleOverlay();
    createMaskImage();
    createRadialShape();

    renderGridLayer();
    renderRadialOverlayMask();

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
        renderEcgLineLayer();
        renderEcgLineMask();
        renderRadialOverlayLayer();
        masker.drawLayer(2);
    }
    post.end();
    post.draw();
    masker.drawOverlay();
}

void erEcgVisualization::createMaskImage(){
    maskImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    for(int x = 0; x < ofGetWidth(); x++){
        for(int y = 0; y < ofGetHeight(); y++){
            float begin = ofGetWidth() * ECG_LINE_TAIL_FROM_LEFT;
            float end = ofGetWidth() * ECG_LINE_TAIL_TO_RIGHT;
            float brightness = ofMap(x, begin, end, 0, 255);
            color = ofColor::fromHsb(0, 0, brightness);
            maskImage.setColor(x, y, color);
        }
    }
    maskImage.update();
}

void erEcgVisualization::createRadialShape(){
    shapeSystem.setup();
    radialShape.setupGradientRing(60, -(ofGetHeight() * ECG_RADIAL_MULTIPLIER), ofGetHeight() * ECG_RADIAL_MULTIPLIER);
    radialShape.setPosition(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    shapeSystem.add(radialShape);
}

void erEcgVisualization::renderGridLayer(){
    currentRow = 0;
    gridIncrement.x = ofGetWidth() / ECG_GRID_DIVISIONS_X;
    gridIncrement.y = ofGetHeight() / ECG_GRID_DIVISIONS_Y;

    masker.beginLayer(0);
    {
        ofBackground(ofColor::black);
        ofSetLineWidth(2);
        ofSetColor(ofColor(5, 70, 190, 182));
        for(int x = gridIncrement.x; x < ofGetWidth(); x += gridIncrement.x){
            for(int y = gridIncrement.y; y < ofGetHeight(); y += gridIncrement.y){
                ofDrawLine(x, 0, x, ofGetHeight());
                ofDrawLine(0, y, ofGetWidth(), y);
            }
        }
    }
    masker.endLayer(0);
}

void erEcgVisualization::renderEcgLineLayer(){
    masker.beginLayer(1);
    {
        ofClear(ofColor(ofColor::black, 0));
        ofSetCurrentRenderer(shivaRenderer);
        ofSetLineWidth(9);
        ofSetColor(ofColor(234, 242, 255));
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
    masker.endLayer(1);
}

void erEcgVisualization::renderEcgLineMask(){
    masker.beginMask(1);
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        maskImage.draw(points.back().x, 0);
        maskImage.draw(points.back().x - ofGetWidth(), 0);
    }
    masker.endMask(1);
}

void erEcgVisualization::renderRadialOverlayLayer(){
    masker.beginLayer(2);
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        masker.drawLayers(0, 1);
    }
    masker.endLayer(2);
}

void erEcgVisualization::renderRadialOverlayMask(){
    masker.beginMask(2);
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        shapeSystem.draw();
    }
    masker.endMask(2);
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