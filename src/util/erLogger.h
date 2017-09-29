#pragma once
#include "ofMain.h"

extern string erProgramId;
extern bool erFileLoggingEnabled;

void erSetLogLevels();
void erEnableFileLogging(bool isServer);
void erDisableFileLogging();
void erLog(string methodSignature, string message);
