#ifndef HEADER
#define HEADER

#include "read_bytes.h"
using namespace std;

class Header {

    private :
        char* chunk_id;
        uint32_t chunk_size;
        uint16_t format_type;
        uint16_t nb_tracks;
        uint16_t time_division; // ticks per quarter-note 

    private :
        void setTimeDivision(uint8_t *data);

    public :
        Header(const char* filename);
        // ~Header();

        friend ostream & operator << (ostream &out, const Header &h);
        
        // ========== getters ===========
        char* getId() const { return chunk_id; }
        uint32_t getSize() const { return chunk_size; }
        uint16_t getFormat() const { return format_type; }
        uint16_t getNbTracks() const { return nb_tracks; }
        uint16_t getTimeDivision() const { return time_division; }

};
#endif