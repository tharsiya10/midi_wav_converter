#ifndef WRITE_MIDI
#define WRITE_MIDI

#include "monophonic_detection.h"
#include "yin.h"
#include "util.h"
#include "Decoders.h"

using namespace std;


void get_all_notes(vector<pair<int, float>> &samples, vector<float> &audio, int sampleRate);
void get_notes_times(vector<note_time> &resultat, vector<float> &audio, int sampleRate);
void write_midi_event(vector<MidiEvent> &midi_events, vector<note_time> &notes);
void write_var_len(int64_t deltaTime, int &deltaLength, std::ofstream &file);
void write_midiTrack(std::ofstream &file, vector<MidiEvent> &midi_events);
void write_endOfTrack(std::ofstream &file);
void write_header(std::ofstream &file);
void write_uint16_be(ofstream &file, uint16_t value);
void write_uint32_be(ofstream &file, uint32_t value);
void write_to_midi(char *filename1, char *filename2);

#endif