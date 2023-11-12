#include "write_wav.h"

/*
* extract information about notes from midi_events -> start time, end time and frequency
*/
void get_notes(vector<note_time> &notes, vector<MidiEvent*> &events, uint64_t time_division){
    // float ticks_par_quart_de_temps = 480.0f; // header : time_division
	float bpm = 60.0f; 
    float prev_start = 0;
    
    for(size_t i = 0; i < events.size(); i++){
        note_time n;
        
        if(events.at(i)->getEventType() == NOTE_ON && events.at(i)->getSecParam() > 0){
            n.freq = midiNoteToFrequency(events.at(i)->getFirstParam());
            n.start = prev_start + (events.at(i)->getVTime() * (60 / bpm)) / time_division;
            prev_start = n.start;
            for(size_t j = i+1; j < events.size(); j++){
                if(events.at(j)->getFirstParam() == events.at(i)->getFirstParam() 
                && ((events.at(j)->getEventType() == NOTE_ON && events.at(j)->getSecParam() == 0) || events.at(j)->getEventType() == NOTE_OFF) ) {
                    n.end = prev_start + (events.at(j)->getVTime() * (60 / bpm)) / time_division;
                    notes.push_back(n);
                    prev_start = n.end;
                    break;
                }
            }
            
        }

    }
}

/*
create sinewave for notes extracted
*/

void generate_sound(vector<note_time> &notes, char *filename){
    double duration = 0.0;
    int sampleRate = 44100;
    // durée totale de séquence
    for(auto elt : notes){
        
        if(elt.start > duration) {
            duration = elt.end;
        }
    }
    
    vector<sf::Int16> samples;

    auto prev_end = notes[0].end;
    for(auto elt : notes){
        if(elt.start > prev_end) {
            for(int i = 0; i < sampleRate * (elt.start-prev_end); i++){
                samples.push_back(sound::SineWave(i, 0, 0.7));
            }
        }
        prev_end = elt.end;
        for(int i = 0; i < sampleRate * (elt.end-elt.start); i++){
            samples.push_back(sound::SineWave(i, elt.freq, 0.7));
        }
        
    
    }
    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
    sf::Sound sound;
    sound.setBuffer(buffer);
    buffer.saveToFile(filename);
    
    

}

void write_to_wav(Midi *midi, char *filename){
    vector<MidiEvent*> midi_events;
    vector<note_time> notes;

    get_midi_events(midi_events, midi);
    get_notes(notes, midi_events, midi->getHeader()->getTimeDivision());
    generate_sound(notes, filename);
}