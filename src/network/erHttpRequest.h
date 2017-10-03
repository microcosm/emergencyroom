#pragma once
#include "ofMain.h"

class erHttpRequest : public ofThread{

public:
    void send(string _url);

    void threadedFunction(){
        lock();
        ofLoadURL(url);
        unlock();
    }

protected:
    string url;
};
