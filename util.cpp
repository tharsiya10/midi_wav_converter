#include "util.h"

int get_note_from_freq(float freq) {
    return (int) (69 + (12 * log2(freq / 440.0f)));
}

float midiNoteToFrequency(int midiNote) {
    const float a4Frequency = 440.0;
    float delta = static_cast<double>(midiNote - 69);
    float frequency = a4Frequency * pow(2.0, delta / 12.0);
    return frequency;
}

void get_midi_events(vector<MidiEvent*> &events, Midi *midi) {
    vector<Track*> *tmp_tracks = midi->getTracks();
    for(int i = 0; i < (int) tmp_tracks->size(); i++){
        Track* track = midi->getTracks()->at(i);

        for(int j = 0; j < (int) track->getEvents()->size(); j++) {
            Event* event = track->getEvents()->at(j);
            if(dynamic_cast<MidiEvent*>(event) == nullptr){
                continue;
            }
            else {
                MidiEvent* midiEvent = static_cast<MidiEvent*>(event);
                if(midiEvent->getEventType() == NOTE_ON || midiEvent->getEventType() == NOTE_OFF){
                    events.push_back(midiEvent);
                }
            }
        }
    }
}

