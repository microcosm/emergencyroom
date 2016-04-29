#include "erGlitchRenderer.h"

void erGlitchRenderer::setup(){
    initializeChannels();
}

void erGlitchRenderer::newOpeningGlitchPeriod(u_int64_t from, float duration, int channel){
    channelsToOpeningGlitchStarts[channel] = from;
    channelsToOpeningGlitchEnds[channel] = from + duration;
}

void erGlitchRenderer::newClosingGlitchPeriod(u_int64_t from, float duration, int channel){
    channelsToClosingGlitchStarts[channel] = from;
    channelsToClosingGlitchEnds[channel] = from + duration;
}

bool erGlitchRenderer::withinGlitchPeriod(int channel, u_int64_t time){
    if(time > channelsToOpeningGlitchStarts[channel] && time < channelsToOpeningGlitchEnds[channel]){
        return true;
    }

    if(time > channelsToClosingGlitchStarts[channel] && time < channelsToClosingGlitchEnds[channel]){
        return true;
    }

    return false;
}

void erGlitchRenderer::initializeChannels(){
    for(int i = 1; i <= settings.numChannels; i++){
        channelsToOpeningGlitchStarts[i] = 0;
        channelsToOpeningGlitchEnds[i] = 0;
        channelsToClosingGlitchStarts[i] = 0;
        channelsToClosingGlitchEnds[i] = 0;
    }
}