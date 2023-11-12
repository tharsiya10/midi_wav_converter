#ifndef WRITE_WAV_H
#define WRITE_WAV_H

#include "util.h"
#include <cmath>
#include <SFML/Audio.hpp>

void get_notes(vector<note_time> &notes, vector<MidiEvent*> &events, uint64_t time_division);
void generate_sound(vector<note_time> &notes, char *filename);
void write_to_wav(Midi *midi, char *filename);


#endif