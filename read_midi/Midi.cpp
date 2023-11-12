#include "Midi.h"
using namespace std;

Midi::Midi(const char* filename) {
    header = new Header(filename);
    tracks = new vector<Track*>();
    long begin = HEADER_LENGTH;
    int nb_tracks = header->getNbTracks();
    for(int i = 0; i < nb_tracks; i++) {
        Track *t = new Track(filename, begin);
        tracks->push_back(t);
        begin += t->getTrackLength();
    }
}

ostream & operator << (ostream &out, const Midi &m) {
    out << *(m.header);
    for(auto t : *(m.tracks)) {
        out << *(t);
    }
    return out;
}