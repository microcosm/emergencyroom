#include "erPlayParams.h"

void erPlayParams::newCommand(string commandStr){
    reset();
    if(commandStr == ER_COMMAND_STR_PLAY){
        command = ER_COMMAND_PLAY;
    }else if(commandStr == ER_COMMAND_STR_TEST){
        command = ER_COMMAND_TEST;
    }else{
        command = ER_COMMAND_UNKNOWN;
    }
}

void erPlayParams::newTestCommand(){
    reset();
    command = ER_COMMAND_TEST;
}

void erPlayParams::newPlayCommand(){
    reset();
    command = ER_COMMAND_PLAY;
}

bool erPlayParams::isTestCommand(){
    return command == ER_COMMAND_TEST;
}

bool erPlayParams::isPlayCommand(){
    return command == ER_COMMAND_PLAY;
}

bool erPlayParams::isPlayable(){
    return isTestCommand() || isPlayCommand();
}

erCommand erPlayParams::getCommand(){
    return command;
}

string erPlayParams::getCommandStr(){
    if(command == ER_COMMAND_PLAY){
        return ER_COMMAND_STR_PLAY;
    }else if(command == ER_COMMAND_TEST){
        return ER_COMMAND_STR_TEST;
    }else{
        return ER_COMMAND_STR_UNKNOWN;
    }
}

void erPlayParams::setPath(string _path){
    path = _path;
}

void erPlayParams::setDelay(int _delay){
    delay = _delay;
}

void erPlayParams::setSpeed(float _speed){
    speed = _speed;
}

string erPlayParams::getPath(){
    return path;
}

int erPlayParams::getDelay(){
    return delay;
}

float erPlayParams::getSpeed(){
    return speed;
}

string erPlayParams::getArgumentStr(){
    return "speed=" + ofToString(speed);
}

void erPlayParams::reset(){
    command = ER_COMMAND_UNKNOWN;
    delay = 3000;
    speed = 1;
}