#ifndef DETECTION
#define DETECTION

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <cstring>
#include <complex>
#include <cassert>
#include <bits/stdc++.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "util.h"

using namespace std;

void decoupage(vector<vector<float>> &windows, vector<float> &audio, int window_size, int hop_size);
float calculate_energy(vector<float> &audio, int start);
void detect_onsets(vector<float> &audio, vector<float> &start, vector<float> &end, int sampleRate);
int handle_audio(vector<float> &audio, char *filename);

#endif