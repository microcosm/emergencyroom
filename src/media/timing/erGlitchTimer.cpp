#include "erGlitchTimer.h"

void erGlitchTimer::setup(){
    initializeChannels();
}

void erGlitchTimer::newOpeningGlitchPeriod(u_int64_t from, float duration, int channel){
    channelsToOpeningGlitchStarts[channel] = from;
    channelsToOpeningGlitchEnds[channel] = from + duration;
}

void erGlitchTimer::newClosingGlitchPeriod(u_int64_t from, float duration, int channel){
    channelsToClosingGlitchStarts[channel] = from;
    channelsToClosingGlitchEnds[channel] = from + duration;
}

void erGlitchTimer::newIntermediateGlitchPeriod(int i, u_int64_t from, float duration, int channel){
    if(i == 1){
        channelsToIntermediateGlitchStarts1[channel] = from;
        channelsToIntermediateGlitchEnds1[channel] = from + duration;
    }else if(i == 2){
        channelsToIntermediateGlitchStarts2[channel] = from;
        channelsToIntermediateGlitchEnds2[channel] = from + duration;
    }else if(i == 3){
        channelsToIntermediateGlitchStarts3[channel] = from;
        channelsToIntermediateGlitchEnds3[channel] = from + duration;
    }
}

bool erGlitchTimer::withinGlitchPeriod(int channel, u_int64_t time){
    if(time > channelsToOpeningGlitchStarts[channel] && time < channelsToOpeningGlitchEnds[channel]){
        return true;
    }

    if(time > channelsToClosingGlitchStarts[channel] && time < channelsToClosingGlitchEnds[channel]){
        return true;
    }

    if(time > channelsToIntermediateGlitchStarts1[channel] && time < channelsToIntermediateGlitchEnds1[channel]){
        return true;
    }

    if(time > channelsToIntermediateGlitchStarts2[channel] && time < channelsToIntermediateGlitchEnds2[channel]){
        return true;
    }

    if(time > channelsToIntermediateGlitchStarts3[channel] && time < channelsToIntermediateGlitchEnds3[channel]){
        return true;
    }

    return false;
}

void erGlitchTimer::initializeChannels(){
    for(int i = 1; i <= settings.numChannels; i++){
        channelsToOpeningGlitchStarts[i] = 0;
        channelsToOpeningGlitchEnds[i] = 0;
        channelsToClosingGlitchStarts[i] = 0;
        channelsToClosingGlitchEnds[i] = 0;

        channelsToIntermediateGlitchStarts1[i] = 0;
        channelsToIntermediateGlitchEnds1[i] = 0;
        channelsToIntermediateGlitchStarts2[i] = 0;
        channelsToIntermediateGlitchEnds2[i] = 0;
        channelsToIntermediateGlitchStarts3[i] = 0;
        channelsToIntermediateGlitchEnds3[i] = 0;
    }
}