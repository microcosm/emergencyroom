#pragma once
#include "ofMain.h"
#include "erChannelRenderer.h"
#include "erSoundRenderer.h"
#include "erNetwork.h"
#include "erEcgTimer.h"
#include "erSettings.h"

#define COSMOLOGICAL_CONSTANT 500
#define ECG_SYNC_DELAY 3000

class erMediaController{

public:
    void setup(erNetwork* _network);
    void setupEcgMode(erNetwork* _network);
    void update();
    void draw();
    virtual void keyReleased(ofKeyEventArgs& args);

    void playClient(erPlayParams params);
    void playServer(int channel, erPlayParams params);
    void floodServer(erPlayParams params);
    bool isChannelPlaying(int channel);
    void stopAll();

    void setVideoPaths(vector<string>* _videoPaths);
    void setVideoPlayers(map<string, ofPtr<erVideoPlayer>>* _videoPlayers);
    void useSoundRendererFor(vector<string>& audibleVideos);
    void calculateVideoPlaybackVariables(erPlayParams params);
    void calculateSoundPlaybackVariables();
    string getClientVideoState();
    erEcgTimer* getEcgTimer();

protected:
    string selectDecoyPath(erPlayParams params);
    erChannelRenderer channelRenderer;
    erSoundRenderer soundRenderer;
    erNetwork* network;

    vector<string>* allVideoPaths;
    map<string, ofPtr<erVideoPlayer>>* videoPlayers;
    ofPtr<erVideoPlayer> videoPlayer;
    string decoyPath;

    u_int64_t currentTime, startOpeningGlitch, startClosingGlitch;
    float openingGlitchDuration, closingGlitchDuration;
    float videoDuration, videoGlitchTime, bufferTime, halfBufferTime;
    bool syncCommandReceived = false;

    vector<u_int64_t> intermediateGlitches;
    vector<float> intermediateGlitchDurations;
};
