#include "Track.h"
using namespace std;

Track::Track(const char* filename, long begin){
    events = new vector<Event*>();
    meta_data meta {};
    meta.end_of_track = false;
    track = {.meta = &meta};
    
    track.meta->end_of_track = false;
    track.midi_status = 0;
    track.midi_channel = 0;
    FILE *midi_file = fopen(filename, "rb");
    fseek(midi_file, begin, SEEK_SET);
    
    fread(type, 4, 1, midi_file);
    
    if(memcmp(type, "MTrk", 4) != 0) {
        fseek(midi_file, begin, SEEK_SET);
    }
    uint8_t *size = (uint8_t *)malloc(sizeof(uint8_t)*4);
    fread(size, 4, 1, midi_file);
    length = read_4bytes(size);
    
    // read length bytes
    uint8_t *data = (uint8_t *) malloc(sizeof(uint8_t) * length);
    fread(data, length, 1, midi_file);

    extract_events(data);
    
    fclose(midi_file);
    
}

void Track::extract_events(uint8_t* bytes){
    int n = 0;
    int64_t v_time; 
    uint8_t code;
    int count = 0;
    while(track.meta->end_of_track != true) 
    {
        v_time =  read_var_length(bytes, &n); 
        code = bytes[n];
    
        switch(code) {
            // meta event
            case 0xFF :
                {
                    MetaEvent *meta = new MetaEvent((bytes)+n+1, v_time, &track);
                    n = n + meta->getEventLength()+1;
                    events->push_back(meta);
                }
                break;
            // sysexevent
            case 0xF0:
            case 0xF7:
                {
                    SysExEvent *sys_ex = new SysExEvent((bytes)+n+1, v_time);
                    n = n + sys_ex->getEventLength()+1; // +1 ?
                    events->push_back(sys_ex);
                }
                break;
            // midi event does not have specific code
            default :
                {
                    MidiEvent *midi_event = new MidiEvent((bytes)+n, v_time, &track);
                    n =  n + midi_event->getEventLength(); 
                    events->push_back(midi_event);
                }
                break;  
        }
        count ++;
        
    }
    this->track_length = n + 8;
    
}

ostream & operator << (ostream &out, const Track &t) {
    out << "MTrk" << "\nlength: " << t.length << "\n" ;
    for(auto e : *(t.events)) {
        out << *(e);
    }
    return out;
}