#pragma once
#include "ofMain.h"
#include <random>

float erGetVolume(string path);
bool erIsFocusVideo(string path);
bool erContains(string& needle, vector<string>& haystack);
void erShuffle(vector<int>& toShuffle);
void erThrowInvalidPath(string method);
