#include "erSound.h"

void erSound::setup(){
    isSetup = true;
    manager.setup();
    manager.toggleDebugUI();
    massive.setup("Massive", 'aumu', 'NiMa', '-NI-');
    manager.createChain(&chain).link(&massive).toMixer();

    ofAddListener(manager.bpm.beatEvent, this, &erSound::play);
    manager.bpm.start();
}

void erSound::ensureSetup(){
    if(!isSetup){
        setup();
    }
}

void erSound::play(void){
    chain.sendMidiOn(60);
}