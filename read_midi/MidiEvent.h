#ifndef MIDI_EVENT
#define MIDI_EVENT

#include "Event.h"
using namespace std;

class MidiEvent : public Event {

    private :
        uint8_t event_type;
        uint8_t midi_channel;
        uint8_t param1;
        uint8_t param2 = 0;
        int event_length;
    
    public : 
        MidiEvent(uint8_t *bytes, int64_t v_time, struct track_data *track);

        // write constructor
        MidiEvent(int64_t v_time, uint8_t event_type, uint8_t midi_channel, uint8_t param1, uint8_t param2);
        void extract_data(uint8_t *bytes, uint8_t b, int *nb_bytes);
        
        ostream & print(ostream &out);

        // ============= getters ===========
        uint8_t getEventType() const { return event_type; }
        uint8_t getMidiChannel() const { return midi_channel; }
        uint8_t getFirstParam() const { return param1; }
        uint8_t getSecParam() const { return param2; }
        int getEventLength() { return event_length; }
};
#endif