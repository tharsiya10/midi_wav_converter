#include "MetaEvent.h"
using namespace std;

MetaEvent::MetaEvent(uint8_t* bytes, int64_t v_time, struct track_data *data) 
    : Event(v_time) 
{
    // bytes à l'endroit juste apres le statut
    int n = 0;
    meta_type = bytes[n];
    n++; // 1 byte
    
    v_length = read_var_length(bytes, &n); // 1 byte + x bytes 
    extract_data(bytes, &n, data);
    this->event_length = n; // 1 byte + x bytes + v_length bytes
}

void MetaEvent::extract_data(uint8_t *bytes, int *nb_bytes, struct track_data *data) {
    switch(this->meta_type) {
        case 0x00 :
            // sequence number
            data->meta->seq_nb = read_2bytes(bytes + *(nb_bytes));   
            break;
        case 0x01 :
        case 0x02 :
        case 0x05 :
        case 0x06 :
        case 0x07 :
        case 0x7F :
            // informations qui ne nous servent pas
            break;
        case 0x03 :
            // sequence or track name
            data->meta->track_name = (char *) malloc(sizeof(char) * v_length);
            data->meta->track_name = read_string(bytes + *(nb_bytes), v_length);
            break;
        case 0x04 :
            // instrument name 
            data->meta->instrument_name = (char *) malloc(sizeof(char) * v_length);
            data->meta->instrument_name = read_string(bytes + *(nb_bytes), v_length);
            break;
        case 0x20 :
            data->meta->channel_prefix = bytes[*(nb_bytes)];
            break;
        case 0x51 :
            data->meta->tempo = read_3bytes(bytes + *(nb_bytes));
            break;
        case 0x54 :
            data->meta->hour = bytes[*(nb_bytes)];
            data->meta->minute = bytes[*(nb_bytes)+1];
            data->meta->sec = bytes[*(nb_bytes)+2];
            data->meta->frame = bytes[*(nb_bytes)+3];
            data->meta->subframe = bytes[*(nb_bytes)+4];
            break;
        case 0x58 :
            data->meta->numerator = bytes[*(nb_bytes)];
            data->meta->denominator = bytes[*(nb_bytes)+1];
            data->meta->midi_clocks = bytes[*(nb_bytes)+2];
            data->meta->nb_notes = bytes[*(nb_bytes)+3];
            break;
        case 0x59 :
            data->meta->sf = (int8_t) bytes[*(nb_bytes)];
            data->meta->mi = bytes[*(nb_bytes)+1];
            break;
        case 0x2F :
            data->meta->end_of_track = true;
        // default : error
        
    }
    *(nb_bytes) += (int) this->v_length;
}

ostream & MetaEvent::print(ostream &out) {
    out << "meta-event\n\tv_time: " << this->v_time << "\n\ttype: " << (int) this->meta_type << " ";
    switch(this->meta_type) {
        case 0x00 :
            out << "Sequence Number";
            break;
        case 0x01 :
            out << "Text Event";
            break;
        case 0x02 :
            out << "Copyright Notice";
            break;
        case 0x03 :
            out << "Sequence/Track Name";
            break;
        case 0x04 :
            out << "Instrument Name";
            break;
        case 0x05 :
            out << "Lyric Text";
            break;
        case 0x06 :
            out << "Marker Text";
            break;
        case 0x07 :
            out << "Cue Point";
            break;
        case 0x20 :
            out << "Midi Channel prefix assignment";
            break;
        case 0x2F :
            out << "End Of Track";
            break;
        case 0x51 :
            out << "Tempo Setting";
            break;
        case 0x54 :
            out << "SMPTE offset";
            break;
        case 0x58 :
            out << "Time Signature";
            break;
        case 0x59 :
            out << "Key Signature";
            break;
        case 0x7F :
            out << "Sequence specific event";
            break;
        default :
            break;
    }
    out << "\n\tv_length : " << this->v_length << endl; ;
    return out;
}