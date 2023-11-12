#include "MidiEvent.h"
using namespace std; 

// constructor for reading midi event from file
MidiEvent::MidiEvent(uint8_t *bytes, int64_t v_time, struct track_data *track) 
: Event(v_time) 
{
    int n = 0;
    uint8_t b = bytes[n];
    uint8_t tmp_status = (b >> 4) & ((1 << 4) - 1);
    if(tmp_status == NOTE_OFF || tmp_status == NOTE_ON || tmp_status == NOTE_AFTERTOUCH 
    || tmp_status == CONTROLLER || tmp_status == PITCH_BEND 
    || tmp_status == PROGRAM_CHANGE || tmp_status == CHANNEL_AFTERTOUCH ){
        n++;
        track->midi_status = tmp_status;
        this->midi_channel = b & ((1 << 4) - 1);
    }
    else {
        tmp_status = track->midi_status;
        this->midi_channel = track->midi_channel;
    }
    this->event_type = tmp_status;
    
    extract_data(bytes, b, &n);
}

// constructor for building/writing a midi event
MidiEvent::MidiEvent(int64_t v_time, uint8_t event_type, uint8_t midi_channel, uint8_t param1, uint8_t param2) : Event(v_time) {
    this->event_type = event_type;
    this->midi_channel = midi_channel;
    this->param1 = param1;
    this->param2 = param2;
}

void MidiEvent::extract_data(uint8_t *bytes, uint8_t b, int *nb_bytes) {
    
    this->param1 = bytes[*(nb_bytes)];
    *(nb_bytes) += 1;

    switch(event_type) { // all the event type possible
        case NOTE_OFF :
        case NOTE_ON :
        case NOTE_AFTERTOUCH :
        case CONTROLLER :
        case PITCH_BEND :
            this->param2 = bytes[*(nb_bytes)];
            *(nb_bytes) += 1;
            break;
        case PROGRAM_CHANGE :
        case CHANNEL_AFTERTOUCH :
            break;
        default : // erreur
            break;
    }
    this->event_length = *(nb_bytes);
}

ostream & MidiEvent::print(ostream &out) {
    out << "midi-event\n\tv_time: " << this->v_time << "\n\tstatus: " << (int) this->event_type << " ";
    switch(this->event_type) {
        case NOTE_OFF :
            out << "Note Off";
            break;
        case NOTE_ON :
            out << "Note On";
            break;
        case NOTE_AFTERTOUCH :
            out << "Note Aftertouch";
            break;
        case CONTROLLER :
            out << "Controller";
            break;
        case PITCH_BEND :
            out << "Pitch Bend";
            break;
        case PROGRAM_CHANGE :
            out << "Program Change";
            break;
        case CHANNEL_AFTERTOUCH :
            out << "Channel Aftertouch";
            break;
        default :
            break;
    }
    out << "\n\tchannel: " << (int) this->midi_channel << "\n\tparam1: " << (int) this->param1 << "\n\tparam2: " << (int) this->param2 << endl;
    return out;

}