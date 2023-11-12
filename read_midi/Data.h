#ifndef DATA
#define DATA

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>

#define HEADER_LENGTH 14

enum midi_event_type : uint8_t {
    NOTE_OFF = 0x8,
    NOTE_ON = 0x9,
    NOTE_AFTERTOUCH = 0xA,
    CONTROLLER = 0xB,
    PROGRAM_CHANGE = 0xC,
    CHANNEL_AFTERTOUCH = 0xD,
    PITCH_BEND = 0xE
};

struct track_data {
    // meta data
    struct meta_data *meta;
    // midi
    uint8_t midi_status = 0;
    uint8_t midi_channel = 0;
};

struct meta_data {
    uint16_t seq_nb = 0;
    char *track_name = 0;
    char *instrument_name = 0;
    uint8_t channel_prefix = 0;
    uint32_t tempo = 0;
    // smpte offset
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t sec = 0;
    uint8_t frame = 0;
    uint8_t subframe = 0;
    // time signature
    uint8_t numerator = 0; 
    uint8_t denominator = 0;
    uint8_t midi_clocks = 0; // per metronome tick
    uint8_t nb_notes = 0;
    // key signature
    int8_t sf = 0; // nb of sharp or flat
    uint8_t mi = 0; // minor or major key
    bool end_of_track = false;
};

#endif