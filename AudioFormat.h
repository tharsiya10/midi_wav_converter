#ifndef AUDIO_FORMAT
#define AUDIO_FORMAT

// #include "Midi.h"
// #include "Header.h"
// #include "Event.h"
// #include "MetaEvent.h"
// #include "MidiEvent.h"
// #include "MTrkEvent.h"
// #include "SineOscillator.h"
// #include "Track.h"
#include "read_midi/Midi.h"
#include <cmath>
#include <iostream>
#include <cstdio>

using namespace std;

const int BIT_DEPTH = 16;
const float MAX_AMPLITUDE = 127.0;
const double PI = 3.14;

struct note_time {
    float freq = 0;
    float start = 0;
    float end = 0;
};


class AudioFormat {
    private:
    Midi* midi_file;
    public :
        AudioFormat(Midi* m);
        void writeAsBytes( ofstream& file, int value, int byte_size);
        float MidiNoteToFrequency(int midiNote) ;
        // vector<SineOscillator> generateSineOscillators(Midi *midi);
        // void generateSample(ofstream& file, Midi* midi,vector<SineOscillator> sine_oscillators);
        void writeWAVFile(const string& filename, Midi* midi);
        vector<int> getDurationNotes(Midi *midi);
        vector<float> getAmplitudeNote(Midi *midi);
        void getFreqNote(Midi *midi, vector<pair<float,float>> &freqAndAmpl, vector<pair<float,float>> &duration);
        uint64_t getEndDuration(uint8_t noteMidi, int startIndex, vector<Event*> *events);


        void getNotes(vector<note_time> &notes, Midi *midi);
};
#endif 
