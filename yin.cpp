#include "yin.h"
using namespace std;

Yin::Yin(long N) : buffer_size(N), yin_buffer(vector<float>(buffer_size, 0)) {
    proba = 0;
}

// difference for each lag j
void Yin::difference(vector<float> &in){
    double delta = 0;
    for(int i = 0; i < buffer_size; i++){
        for(int j = 0; j < buffer_size; j++){
            delta = in[j] - in[i+j];
            yin_buffer[i] += delta * delta;
        }
    }
}

/* cumulative mean normalized difference
smoothes variations or noises
*/
void Yin::cmndf(){
    float sum = 0;
    yin_buffer[0] = 1;
    
    for(int i = 1; i < buffer_size; i++){
        sum += yin_buffer[i];
        yin_buffer[i] *= i / sum;
        
    }
}

// localisation of threshold
int Yin::absolute_threshold(){
    int i;
    for(i = 2; i < buffer_size; i++){
        if(yin_buffer[i] < THRESHOLD) {
            while(i + 1 < buffer_size 
            && yin_buffer[i+1] < yin_buffer[i]){
                i++;
            }
            proba = 1 - yin_buffer[i];
            break;
        }
    }
    if(i == buffer_size || yin_buffer[i] >= THRESHOLD) {

        i = -1;
        proba = 0;
    }
    
    return i;
}

// precision of threshold estimated
double Yin::parabolic_interpolation(int tau_est) {
    double new_tau;
    int tau_inf, tau_sup;
    if(tau_est < 1) tau_inf = tau_est;
    else tau_inf = tau_est - 1;

    if(tau_est + 1 < buffer_size) { 
        tau_sup = tau_est + 1;
    }
    else { tau_sup = tau_est; }

    if(tau_inf == tau_est) {
        if(yin_buffer[tau_est] <= yin_buffer[tau_sup]) {
            new_tau = tau_est;
        }
        else {
            new_tau = tau_sup;
        }
    }
    else {
        double t0, t1, t2;
        t0 = yin_buffer[tau_inf];
        t1 = yin_buffer[tau_est];
        t2 = yin_buffer[tau_sup];

        // formula of parabole
        new_tau = tau_est + (t2 - t0) / (2 * (2 * t1 - t2 - t0));

    }
    return new_tau;
}

float Yin::get_pitch(vector<float> &in, int sample_rate) {
    
    int tau_est = -1;
    float pitch = -1;

    difference(in);
    cmndf();
    tau_est = absolute_threshold();
    if(tau_est != -1){
        pitch = sample_rate / parabolic_interpolation(tau_est);
        yin_tau = tau_est;
    }
    return pitch;
}

void Yin::clear() {
    proba = 0;
    yin_buffer.clear();
    yin_tau = 0;
}
