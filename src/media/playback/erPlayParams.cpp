#include "erPlayParams.h"

void erPlayParams::newCommand(string commandStr){
    reset();
    if(commandStr == ER_COMMAND_STR_VIDEO){
        command = ER_COMMAND_VIDEO;
    }else if(commandStr == ER_COMMAND_STR_GRAPHIC){
        command = ER_COMMAND_GRAPHIC;
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

void erPlayParams::newGraphicCommand(){
    reset();
    command = ER_COMMAND_GRAPHIC;
}

void erPlayParams::newVideoCommand(){
    reset();
    command = ER_COMMAND_VIDEO;
}

bool erPlayParams::isTestCommand(){
    return command == ER_COMMAND_TEST;
}

bool erPlayParams::isVideoCommand(){
    return command == ER_COMMAND_VIDEO;
}

bool erPlayParams::isGraphicCommand(){
    return command == ER_COMMAND_GRAPHIC;
}

bool erPlayParams::isKnownCommand(){
    return command != ER_COMMAND_UNKNOWN;
}

bool erPlayParams::isPlayable(){
    return isTestCommand() || isVideoCommand();
}

erCommand erPlayParams::getCommand(){
    return command;
}

string erPlayParams::getCommandStr(){
    if(command == ER_COMMAND_VIDEO){
        return ER_COMMAND_STR_VIDEO;
    }else if(command == ER_COMMAND_GRAPHIC){
        return ER_COMMAND_STR_GRAPHIC;
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
    return "path:" + path + ",speed:" + ofToString(speed);
}

void erPlayParams::reset(){
    command = ER_COMMAND_UNKNOWN;
    delay = 3000;
    speed = 1;
}