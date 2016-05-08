#include "erEcgRenderer.h"

//Common resolutions
//MBP retina: 1440 x 900
//Monoprice: 2560 x 1600 / 2048 x 1280 / 1600 x 1000 / 1280 x 800

void erEcgRenderer::setup(erNetwork* _network){
    network = _network;
    timeOffset = 0;

    width = ofGetWidth();
    height = ofGetHeight();

    isPlaying = false;
    ecgTimer.setup();

    lineColor = ofColor(234, 242, 255);
    masker.setup(width, height, 3);
    masker.toggleOverlay();
    createLinearMaskImage();
    createRadialMaskShape();
    createLineHeadShape();

    renderGridLayer();
    renderRadialOverlayMask();

    shivaRenderer = ofPtr<ofxShivaVGRenderer>(new ofxShivaVGRenderer);
    defaultRenderer = ofGetCurrentRenderer();
    shivaRenderer->setLineCapStyle(VG_CAP_ROUND);
    post.init();
    //post.createPass<FxaaPass>();
    //post.createPass<BloomPass>();

    readData();

    ofAddListener(ofEvents().update, this, &erEcgRenderer::update);
    ofAddListener(ofEvents().draw, this, &erEcgRenderer::draw);
    ofAddListener(network->clientMessageReceived(), this, &erEcgRenderer::messageReceived);
}

void erEcgRenderer::update(ofEventArgs& args){
    if(isPlaying){
        ecgTimer.update();

        lastRow = currentRow;
        lastPeriodPosition = periodPosition;
        periodPosition = ecgTimer.getPeriodPosition();
        currentRow = ofMap(periodPosition, 0, 1, 0, numRows-1);

        loadNewPoints();
        trimPointsToSize();
    }
}

void erEcgRenderer::draw(ofEventArgs& args){
    ofBackground(ofColor::black);
    if(isPlaying && points.size() > 0){
        post.begin();
        {
            renderEcgLineLayer();
            renderEcgLineMask();
            renderRadialOverlayLayer();
            masker.drawLayer(2);
        }
        post.end();
        post.draw();
    }
    if(settings.clientDrawingEnabled){
        masker.drawOverlay();
        if(scheduled){
            ofDrawBitmapString("SYNCING...", 10, 72);
        }
    }
}

void erEcgRenderer::messageReceived(string& message){
    if(message.substr(0, 7) == "ECGSYNC"){
        schedule(ECG_SYNC_DELAY);
    }
}

void erEcgRenderer::readData(){
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

void erEcgRenderer::loadNewPoints(){
    for(int n = lastRow; n <= currentRow; n++){
        nthPeriodPosition = ofMap(n, lastRow, currentRow, lastPeriodPosition, periodPosition);
        point.x = ofMap(nthPeriodPosition, 0, 1, 0, width);
        point.y = ofMap(data.at(n), ECG_HIGHEST_VALUE, ECG_LOWEST_VALUE, 0, height);
        if(n > lastRow){
            points.push_back(point);
        }
    }
}

void erEcgRenderer::trimPointsToSize(){
    if(points.size() > ECG_MAX_POINTS){
        points.erase(points.begin(), points.begin() + points.size() - ECG_MAX_POINTS);
    }
}

void erEcgRenderer::createLinearMaskImage(){
    linearMaskImage.allocate(width, height, OF_IMAGE_COLOR);
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            float begin = width * ECG_LINE_TAIL_FROM_LEFT;
            float end = width * ECG_LINE_TAIL_TO_RIGHT;
            float brightness = ofMap(x, begin, end, 0, 255);
            ofColor color = ofColor::fromHsb(0, 0, brightness);
            linearMaskImage.setColor(x, y, color);
        }
    }
    linearMaskImage.update();
}

void erEcgRenderer::createRadialMaskShape(){
    radialShapeSystem.setup();
    radialShape.setupGradientRing(60, -(height * ECG_RADIAL_MULTIPLIER), height * ECG_RADIAL_MULTIPLIER);
    radialShape.setPosition(width * 0.5, height * 0.5);
    radialShapeSystem.add(radialShape);
}

void erEcgRenderer::createLineHeadShape(){
    lineHeadShapeSystem.setup();
    lineHeadShape.setupGradientRing(20, -ECG_LINE_HEAD_SIZE, ECG_LINE_HEAD_SIZE);
    lineHeadShapeSystem.add(lineHeadShape);
}

void erEcgRenderer::renderGridLayer(){
    currentRow = 0;
    gridIncrement.x = width / ECG_GRID_DIVISIONS_X;
    gridIncrement.y = height / ECG_GRID_DIVISIONS_Y;

    masker.beginLayer(0);
    {
        ofBackground(ofColor::black);
        ofSetLineWidth(2);
        ofSetColor(ofColor(5, 70, 190, 182));
        for(int x = gridIncrement.x; x < width; x += gridIncrement.x){
            for(int y = gridIncrement.y; y < height; y += gridIncrement.y){
                ofDrawLine(x, 0, x, height);
                ofDrawLine(0, y, width, y);
            }
        }
    }
    masker.endLayer(0);
}

void erEcgRenderer::renderEcgLineLayer(){
    masker.beginLayer(1);
    {
        ofClear(ofColor(ofColor::black, 0));
        ofSetCurrentRenderer(shivaRenderer);
        ofSetLineWidth(9);
        ofSetColor(lineColor);
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

void erEcgRenderer::renderEcgLineMask(){
    masker.beginMask(1);
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        linearMaskImage.draw(points.back().x, 0);
        linearMaskImage.draw(points.back().x - width, 0);
    }
    masker.endMask(1);
}

void erEcgRenderer::renderRadialOverlayLayer(){
    masker.beginLayer(2);
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        masker.drawLayers(0, 1);
        ofSetColor(lineColor);
        lineHeadShape.setPosition(points.back().x, points.back().y);
        lineHeadShapeSystem.draw();
    }
    masker.endLayer(2);
}

void erEcgRenderer::renderRadialOverlayMask(){
    masker.beginMask(2);
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor::white);
        radialShapeSystem.draw();
    }
    masker.endMask(2);
}