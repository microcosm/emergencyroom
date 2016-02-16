#pragma once
#include "ofMain.h"

#define ER_COMMAND_STR_PLAY "PLAY"
#define ER_COMMAND_STR_TEST "TEST"
#define ER_COMMAND_STR_UNKNOWN "UNKNOWN"

enum erCommand{
    ER_COMMAND_PLAY,
    ER_COMMAND_TEST,
    ER_COMMAND_UNKNOWN
};

class erPlayParams{

public:
    void newCommand(string commandStr);
    void newTestCommand();
    void newPlayCommand();
    bool isTestCommand();
    bool isPlayCommand();
    bool isPlayable();
    erCommand getCommand();
    string getCommandStr();
    void setPath(string _path);
    void setDelay(int _delay);
    void setSpeed(float _speed);
    string getPath();
    int getDelay();
    float getSpeed();
    string getArgumentStr();

protected:
    void reset();
    erCommand command;
    int delay;
    float speed;
    string path;
};

