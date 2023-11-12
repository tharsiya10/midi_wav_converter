#ifndef COMPARE
#define COMPARE

// #define PITCH_TOLERANCE 3
// #define VTIME_TOLERANCE 480

// #include "read_midi/Midi.h"
#include "util.h"


// void get_midi_events(vector<MidiEvent> events, Midi *midi);
void get_similarity(Midi *m1, Midi *m2);

#endif