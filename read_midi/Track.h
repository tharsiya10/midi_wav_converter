#ifndef TRACK
#define TRACK

#include "read_bytes.h"
#include "Data.h"
#include "MetaEvent.h"
#include "SysExEvent.h"
#include "MidiEvent.h"
#include <vector>
#include <cstring>
using namespace std;

class Track {

    private :   
        char type[4];
        uint32_t length;
        vector<Event*> *events;
        struct track_data track;
        uint64_t track_length; // total length
        
    public :
        Track(const char* filename, long begin);
        void extract_events(uint8_t* bytes);

        friend ostream & operator << (ostream &out, const Track &t);

        // ============= getters ===========
        char* getType() { return type; }
        uint32_t getLength() { return length; }
        vector<Event*>* getEvents() { return events; }
        uint64_t getTrackLength() const { return track_length; }
};
#endif