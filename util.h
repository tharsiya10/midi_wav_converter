#ifndef UTIL_H
#define UTIL_H

#include "read_midi/Midi.h"
#include <cmath>

#define WIN_SIZE 4096
#define HOP_SIZE 2048
#define ENERGY_THRESHOLD 200

#define PITCH_TOLERANCE 1
#define VTIME_TOLERANCE 480


struct note_time {
    float freq = 0;
    float start = 0;
    float end = 0;
};

namespace sound {

	inline short SineWave(double time, double freq, double amp) {
		
		double tpc = 44100 / freq; // ticks per cycle
		double cycles = time / tpc;
		short amplitude = 32767 * amp;
		short result = amplitude * sin(2.0 * M_PI * cycles);
		return result;
	}
}

int get_note_from_freq(float freq);
float midiNoteToFrequency(int midiNote);
void get_midi_events(vector<MidiEvent*> &events, Midi *midi);


#endif