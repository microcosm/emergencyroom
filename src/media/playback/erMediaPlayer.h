#pragma once
#include "ofMain.h"
#include "erChannelRenderer.h"
#include "erSoundRenderer.h"
#include "erTextRenderer.h"
#include "erNetwork.h"
#include "erSettings.h"

#define COSMOLOGICAL_CONSTANT 500
#define ECG_SYNC_DELAY 3000

class erMediaPlayer{

public:
    void setup(erNetwork* _network);
    void setupEcgMode(erNetwork* _network);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    virtual void keyReleased(ofKeyEventArgs& args);

    void playClient(erPlayParams params);
    void playServer(int channel, erPlayParams params);
    bool isChannelPlaying(int channel);

    void setVideoPaths(vector<string>* _videoPaths);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);
    void setTexts(map<string, vector<string>>* texts);
    void useSoundRendererFor(vector<string>& audibleVideos);
    void calculateVideoPlaybackVariables(erPlayParams params);
    void calculateSoundPlaybackVariables();
    string getClientVideoState();

protected:
    ofPtr<erSyncedVideoPlayer> getDecoyPlayer();
    erChannelRenderer channelRenderer;
    erSoundRenderer soundRenderer;
    erTextRenderer textRenderer;
    erNetwork* network;

    vector<string>* allVideoPaths;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;
    ofPtr<erSyncedVideoPlayer> videoPlayer;

    u_int64_t currentTime, startOpeningGlitch, startClosingGlitch, startText, startTextOverlay;
    float openingGlitchDuration, closingGlitchDuration, textDuration, textOverlayDuration;
    float videoDuration, videoGlitchTime, bufferTime, halfBufferTime;
    bool renderText;

    vector<u_int64_t> intermediateGlitches;
    vector<float> intermediateGlitchDurations;
};