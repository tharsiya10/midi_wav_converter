#include "Header.h"
#include <bitset>
using namespace std;

Header::Header(const char* filename) {
    FILE *midi_file = fopen(filename, "rb");
    chunk_id = (char*) malloc(sizeof(char)*4);
    fread(chunk_id, 4, 1, midi_file);
    
    uint8_t *size = (uint8_t*) malloc(sizeof(uint8_t) * 4);
    fread(size, 4, 1, midi_file);

    uint8_t *buf = (uint8_t*) malloc(sizeof(uint8_t) * 6);
    fread(buf, 6, 1, midi_file);

    this->chunk_size = read_4bytes(size);

    this->format_type = read_2bytes(buf);
    this->nb_tracks = read_2bytes(buf+2);
    
    
    setTimeDivision(buf+4);

    
    fclose(midi_file); 
}

void Header::setTimeDivision(uint8_t *data){
    
    bitset<16> b(read_2bytes(data));
    if(b[15] == 0){
        this->time_division = read_2bytes(data);
    }
    else {
        cout << "frames per second" << endl;
        int n = 0;
        this->time_division = read_delta_time(data, &n);
    }
}

ostream & operator << (ostream &out, const Header &h) {
    return out << "MThd\n\tchunk_size: " << (int) h.chunk_size 
    << "\n\tformat_type: " << (int) h.format_type << "\n\tnb_tracks: " 
    << (int) h.nb_tracks << "\n\ttime-division: " << (int) h.time_division << endl;
}