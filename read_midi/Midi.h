#ifndef MIDI
#define MIDI

#include "Header.h"
#include "Track.h"
#include <vector>
#include <cstring>
using namespace std;

class Midi {

    private :
        Header *header;
        vector<Track*> *tracks;

    public :
        Midi(const char* filename);

        friend ostream & operator << (ostream &out, const Midi &m);

        // ================= getters =============
        Header* getHeader() const { return header; }
        vector<Track*> *getTracks() const { return tracks; }
};

#endif