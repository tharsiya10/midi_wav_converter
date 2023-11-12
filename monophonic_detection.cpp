#include "monophonic_detection.h"
#include "yin.h"
#include "Decoders.h"

using namespace std;

void decoupage(vector<vector<float>> &windows, vector<float> &audio, int window_size, int hop_size) {
    int num_win = ceil((double) (audio.size() - window_size) / hop_size) + 1;

    for(int i = 0; i < num_win; i++){
        int start_index = i * hop_size;
        for(int j = 0; j < window_size; j++){
            if(start_index + j < (int) audio.size()) {
                windows[i][j] = audio[start_index + j];
            }
            else {
                windows[i][j] = 0;
            }
        }
    }
}

// calculate peak of energy
float calculate_energy(vector<float> &audio, int start) {
    float energy = 0.0;
    for(int i = start; i < start + WIN_SIZE; i++){
        energy += audio[i] * audio[i];
    }
    return energy;
}

// detection beginning and ending of notes with energy values
void detect_onsets(vector<float> &audio, vector<float> &start, vector<float> &end, int sampleRate) {
    int size = (int) audio.size();
    float curr_time = 0;
    float start_time = 0;
    float prev_energy = 0;
    
    for(int i = 0; i < size - WIN_SIZE; i += WIN_SIZE) {
        float energy = calculate_energy(audio, i);
        
        if(energy > ENERGY_THRESHOLD) {
            
            if(start_time == 0){
                start_time = curr_time;
            }
            
            prev_energy = curr_time - start_time;
            start.push_back(prev_energy);
        }
        else {
            if((prev_energy - energy) < 2 * ENERGY_THRESHOLD) {
                
                end.push_back(curr_time - start_time);
            }
        }
        curr_time += (float) WIN_SIZE / (float) sampleRate;
    }
}

// returns sampleRate
int handle_audio(vector<float> &audio, char *filename) {
    // load audio 
    nqr::NyquistIO loader;
    shared_ptr<nqr::AudioData> file_data = make_shared<nqr::AudioData>();
    
    loader.Load(file_data.get(), filename);

    transform(file_data->samples.begin(), file_data->samples.begin(), file_data->samples.end(), file_data->samples.begin(),
    bind(multiplies<float>(), placeholders::_1, 10000));

    // transforming into mono if not already
    if(file_data->channelCount == 2){
        vector<float> audio_copy(file_data->samples.size() / 2);
        nqr::StereoToMono(file_data->samples.data(), audio_copy.data(), file_data->samples.size());
        audio = vector<float>(audio_copy.begin(), audio_copy.end());
    }
    return file_data->sampleRate;
}



