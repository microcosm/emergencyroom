#pragma once
#include "ofMain.h"
#include "erChannelRenderer.h"
#ifdef __APPLE__
    #include "erSoundRenderer.h"
#endif
#include "erTextRenderer.h"
#include "erNetwork.h"
#include "erEcgTimer.h"
#include "erSettings.h"

#define COSMOLOGICAL_CONSTANT 500
#define ECG_SYNC_DELAY 3000

class erMediaPlayer{

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
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);
    void setTexts(map<string, vector<string>>* texts);
    void useSoundRendererFor(vector<string>& audibleVideos);
    void calculateVideoPlaybackVariables(erPlayParams params);
    void calculateSoundPlaybackVariables();
    string getClientVideoState();
    erEcgTimer* getEcgTimer();

protected:
    string selectDecoyPath(erPlayParams params);
    erChannelRenderer channelRenderer;
    #ifdef __APPLE__
        erSoundRenderer soundRenderer;
    #endif
    erTextRenderer textRenderer;
    erNetwork* network;

    vector<string>* allVideoPaths;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;
    ofPtr<erSyncedVideoPlayer> videoPlayer;
    string decoyPath;

    u_int64_t currentTime, startOpeningGlitch, startClosingGlitch, startText, startTextOverlay;
    float openingGlitchDuration, closingGlitchDuration, textDuration, textOverlayDuration;
    float videoDuration, videoGlitchTime, bufferTime, halfBufferTime;
    bool renderText;
    bool syncCommandReceived = false;

    vector<u_int64_t> intermediateGlitches;
    vector<float> intermediateGlitchDurations;
};
