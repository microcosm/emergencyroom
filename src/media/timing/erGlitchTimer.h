#pragma once
#include "ofMain.h"
#include "erSettings.h"

class erGlitchTimer{

public:
    void setup();
    void newOpeningGlitchPeriod(u_int64_t from, float duration, int channel);
    void newClosingGlitchPeriod(u_int64_t from, float duration, int channel);
    void newIntermediateGlitchPeriod(int i, u_int64_t from, float duration, int channel);
    bool withinGlitchPeriod(int channel, u_int64_t time=ofGetElapsedTimeMillis());

protected:
    void initializeChannels();
    map<int, u_int64_t> channelsToOpeningGlitchStarts;
    map<int, u_int64_t> channelsToOpeningGlitchEnds;
    map<int, u_int64_t> channelsToClosingGlitchStarts;
    map<int, u_int64_t> channelsToClosingGlitchEnds;

    map<int, u_int64_t> channelsToIntermediateGlitchStarts1;
    map<int, u_int64_t> channelsToIntermediateGlitchEnds1;
    map<int, u_int64_t> channelsToIntermediateGlitchStarts2;
    map<int, u_int64_t> channelsToIntermediateGlitchEnds2;
    map<int, u_int64_t> channelsToIntermediateGlitchStarts3;
    map<int, u_int64_t> channelsToIntermediateGlitchEnds3;
};