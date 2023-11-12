#include "write_midi.h"

using namespace std;

/*
splitting audio in windows 
get pitch from each window if possible
*/
void get_all_notes(vector<pair<int, float>> &samples, vector<float> &audio, int sampleRate) {
    int num_win = ceil((double) (audio.size() - WIN_SIZE) / HOP_SIZE) + 1;
    vector<vector<float>> windows(num_win, vector<float>(WIN_SIZE, 0));
    
    decoupage(windows, audio, WIN_SIZE, HOP_SIZE);

    int window_index = 0;
    float freq;
    Yin yin(WIN_SIZE);
    for(auto window : windows) {
        freq = yin.get_pitch(window, sampleRate);
        
        if(yin.proba != 0) {
            
            auto res = make_pair(window_index, freq);
            samples.push_back(res);
        }
        yin.clear();
        window_index ++;
        
    }
}

/*
associate each note to a start and end detected by onset and offset

*/
void get_notes_times(vector<note_time> &resultat, vector<float> &audio, int sampleRate) {
    vector<pair<int,float>> notes;
    vector<float> start;
    vector<float> end;
    get_all_notes(notes, audio, sampleRate);
    detect_onsets(audio, start, end, sampleRate);

    int ind_start = 0, ind_end = 0, ind_note = 0;
    cout << "RESULTATS" << endl;
    float prev_end = -1;
    while(ind_note < (int) notes.size()) {
        // window time where frequency was detected
        float start_window = notes[ind_note].first * HOP_SIZE / sampleRate; 
        float end_window = (notes[ind_note].first + 1) * HOP_SIZE / sampleRate; 
        note_time n;
        
        // if start is in window time then association
        if(start[ind_start] >= start_window && start[ind_start] < end_window 
        && (start[ind_start] > prev_end)) {
            n.freq = notes[ind_note].second;
            n.start = start[ind_start];
            cout << "===NOTE===" << endl;
            cout << n.freq << endl;
            cout << n.start << endl;
            
            while(ind_end < (int) end.size() && end[ind_end] <= start[ind_start]) {
                ind_end ++;
            }
            if(ind_end < (int) end.size()) {
                n.end = end[ind_end];
                cout << n.end << endl;
            }
            else {
                n.end = -1;
            }
            
            resultat.push_back(n);
            ind_start ++;
            ind_end ++;
            
            ind_note ++;
            prev_end = n.end;
        }
        else if(start[ind_start] < start_window || start[ind_start] < prev_end) {
            
            ind_start ++;
        }
        else {
            
            ind_note ++;
        }
    }
}

void write_midi_event(vector<MidiEvent> &midi_events, vector<note_time> &notes) {
    if(notes.empty()) { return; } 
    float prev_time = notes[0].start;
    for(auto note : notes) {
        uint64_t v_time1 = (uint64_t) ((note.start - prev_time) * 480.0f * (120.0f / 60.0f));
        uint8_t event = 0x9;
        uint8_t midi_channel = 0;
        uint8_t param1 = get_note_from_freq(note.freq);
        uint8_t param2 = 90;
        prev_time = note.start;
        MidiEvent deb(v_time1, event, midi_channel, param1, param2);
        uint64_t v_time2 = (uint64_t) ((note.end - prev_time) * 480.0f * (120.0f / 60.0f));
        MidiEvent fin(v_time2, event, midi_channel, param1, 0); 
        midi_events.push_back(deb);
        midi_events.push_back(fin);
        prev_time = note.end;
    }
}

// variable length quantity for deltaTime (relative time at the beginning of each midi_event)
void write_var_len(uint64_t deltaTime, int &deltaLength, std::ofstream &file){
    uint64_t buffer;
    buffer = deltaTime & 0x7F;

    while((deltaTime >>= 7)) {
        buffer <<= 8;
        buffer |= ((deltaTime & 0x7F) | 0x80);
    }

    while(true) {
        deltaLength ++;
        file.put(buffer);
        if(buffer & 0x80) {
        
            buffer >>= 8;
        }
        else {
            break;
        }
    }
}

void write_midiTrack(std::ofstream &file, vector<MidiEvent> &midi_events){
    // write track beginning MTrK
    file << char(0x4D) << char(0x54) << char(0x72) << char(0x6B);
    uint32_t length = 0;
    file.write((char *)&length, sizeof(length));
    uint64_t deltaTime = 0;
    int deltaLength = 0;
    for(auto event : midi_events) {
        deltaTime = event.getVTime();
        
        write_var_len(deltaTime, deltaLength, file);
        length += deltaLength;
        uint8_t eventAndChannel = (event.getEventType() << 4) | (event.getMidiChannel());
        
        file.write((char*) &eventAndChannel, 1);

        uint8_t param1 = event.getFirstParam();
        uint8_t param2 = event.getSecParam();
        file.write((char*) &param1, sizeof(uint8_t));
        file.write((char*) &param2, sizeof(uint8_t));

        length += 3;
    }

    write_endOfTrack(file);
    
    file.seekp(14 + 4);
    write_uint32_be(file, length);

}

void write_uint16_be(ofstream &file, uint16_t value){
    uint8_t bytes[2];
    bytes[0] = (value >> 8) && 0xFF;
    bytes[1] = (value) && 0xFF;
    file.write((char*) (bytes), 2);

}

void write_uint32_be(ofstream &file, uint32_t value) {
    uint8_t bytes[4];
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = (value) & 0xFF;
    file.write((char*)(bytes), 4);
}

void write_endOfTrack(std::ofstream &file){
    file << char(0x00) << char(0xFF) << char(0x2F) << char(0x00);
}

void write_header(std::ofstream &file) {
    uint16_t midi_format = 0;
    uint16_t num_tracks = (uint16_t) 1;
    uint16_t time_division = 480;

    uint16_t num_tracks_be = (num_tracks << 8) | (num_tracks >> 8);
    uint16_t time_division_be = (time_division << 8) | (time_division >> 8);

    file << char(0x4D) << char(0x54) << char(0x68) << char(0x64); // MThK
    file << char(0x00) << char(0x00) << char(0x00) << char(0x06); // length 6 -> size of next elements in header
    
    file.write(reinterpret_cast<const char*>(&midi_format), 2);
    file.write(reinterpret_cast<const char*>(&num_tracks_be), 2);
    file.write(reinterpret_cast<const char*>(&time_division_be), 2);

}

void write_to_midi(char *filename1, char* filename2) {
    vector<float> audio;
    int sampleRate = handle_audio(audio, filename1);

    vector<note_time> notes;
    vector<MidiEvent> midi_events;
    get_notes_times(notes, audio, sampleRate);
    write_midi_event(midi_events, notes);
    cout << "Nombre de notes : " << notes.size() << endl;

    ofstream file(filename2, ios::out | ios::binary);
    if(!file) {
        cerr << "Impossible d'ouvrir fichier en écriture" << endl;
        return;
    }
    write_header(file);
    write_midiTrack(file, midi_events);
    file.close();
}
