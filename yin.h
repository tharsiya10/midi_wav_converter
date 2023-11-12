#ifndef YIN
#define YIN

#define THRESHOLD 0.1
#define MIN_FREQ 20
#define MAX_FREQ 20000

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <sndfile.h>
#include <vector>
#include <cstring>
#include <complex>
#include <cassert>
#include <math.h>
#include <limits.h>
#include <bits/stdc++.h>


using namespace std;

class Yin {
    public :
        long buffer_size;
        vector<float> yin_buffer;
        int yin_tau = 0;     

    public :
        double proba;
        Yin(long N);        
        void clear();
        void difference(vector<float> &in);
        void cmndf();
        int absolute_threshold();
        double parabolic_interpolation(int tau_est);
        float get_pitch(vector<float> &in, int sample_rate);
};

#endif