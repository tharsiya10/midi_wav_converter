#include "AudioFormat.h"


AudioFormat::AudioFormat(Midi* m){midi_file=m;}



void AudioFormat::writeAsBytes( ofstream& file, int value, int byte_size){
    file.write(reinterpret_cast<const char*>(&value), byte_size);
}

float AudioFormat::MidiNoteToFrequency(int midiNote) {
    const float a4Frequency = 440.0;
    float delta = static_cast<double>(midiNote - 69);
    float frequency = a4Frequency * pow(2.0, delta / 12.0);
    return frequency;
}

uint64_t AudioFormat::getEndDuration(uint8_t noteMidi, int startIndex, vector<Event*> *events) {
	for(int i = startIndex + 1; i < (int) events->size(); i++) {
		Event* event = events->at(i);
		if (dynamic_cast<MidiEvent*>(event) == nullptr) { continue; }
		else {
			MidiEvent* midiEvent = static_cast<MidiEvent*>(event);
			if(midiEvent->getFirstParam() == noteMidi && ((midiEvent->getEventType() == NOTE_ON && midiEvent->getSecParam() == 0) ||  (midiEvent->getEventType() == NOTE_OFF))) {
				return midiEvent->getVTime();
			}
		}
	}
	return 0;		
}


void AudioFormat::getFreqNote(Midi *midi, vector<pair<float,float>> &freqAndAmpl, vector<pair<float,float>> &duration){ 
	
	float ticks_par_quart_de_temps = 480.0f; // header : time_division
	float bpm = 120.0f; // ?
	
    vector<Track*> *tmp_tracks=midi->getTracks();
    for (int i = 0; i < (int) tmp_tracks->size();i++){
        Track* track = midi->getTracks()->at(i);
        for (int j = 0; j < (int) track->getEvents()->size(); j++) {
            Event* event = track->getEvents()->at(j);
            if (dynamic_cast<MidiEvent*>(event) == nullptr) {
                continue;
            }else{
                MidiEvent* midiEvent = static_cast<MidiEvent*>(event);
                if (midiEvent->getEventType() == NOTE_ON && midiEvent->getSecParam()!=0) {
                    float tmp_noteFreq = MidiNoteToFrequency(midiEvent->getFirstParam());
                    float tmp_ampl = midiEvent->getSecParam() ;
                    
                    freqAndAmpl.push_back(make_pair(tmp_noteFreq, tmp_ampl / 100));
                    int64_t note_start = midiEvent->getVTime();
                    int64_t note_end = getEndDuration(midiEvent->getFirstParam(), j, track->getEvents());
                    
                    int64_t diff = (note_end - note_start); 
                    float delta = (diff * (60 / bpm)) / ticks_par_quart_de_temps;
                    cout << "NOTE_START : " << note_start << " NOTE_END : " << note_end << " DURATION : " << delta << endl; 
                    duration.push_back(make_pair(note_start, delta));
                    
                }
            }
        }
    }
}

void AudioFormat::getNotes(vector<note_time> &notes, Midi* midi){

}

/*
void AudioFormat::generateSample(ofstream& file, Midi* midi,vector<SineOscillator> sine_oscillators){
    vector<int> durations = getDurationNotes(midi);
    cout << durations.size() << endl;
    if (durations.size() != sine_oscillators.size()) cout << "Error";
    for (int i = 0 ; i < sine_oscillators.size();i++){
        for (int j = 0 ; j < SAMPLE_RATE ; j++) {
            auto sample = sine_oscillators.at(i).process();
            int intSample = static_cast<int>(sample*MAX_AMPLITUDE);
            writeAsBytes(file, intSample, 4);
        }
    }
}


void AudioFormat::writeWAVFile(const string& filename, Midi* midi){
    ofstream wavFile;
    wavFile.open(filename, ios::binary);
    wavFile<<"RIFF";
    wavFile<<"----";
    wavFile<<"WAVE";
    wavFile<<"fmt ";
    writeAsBytes(wavFile,16,4); // subchunk size
    writeAsBytes(wavFile,1,2); //audio format
    writeAsBytes(wavFile,1,2); // num channel 1=mono
    writeAsBytes(wavFile,SAMPLE_RATE,4);
    int byte_rate = SAMPLE_RATE * 1 * 16 / 8;
    int block_align = 1 * (16 / 8);
    writeAsBytes(wavFile,byte_rate,4);
    writeAsBytes(wavFile,block_align,2);
    writeAsBytes(wavFile,16,2);//bit per sample
    wavFile<<"data";
    wavFile<<"----";
    int start_audio = wavFile.tellp();
    vector<SineOscillator> sine_osc= generateSineOscillators(midi);
    generateSample(wavFile,midi,sine_osc);
    int end_audio = wavFile.tellp();
    wavFile.seekp(start_audio-4);
    writeAsBytes(wavFile,end_audio-start_audio,4);
    wavFile.seekp(4, ios::beg);
    writeAsBytes(wavFile,end_audio-8,4);
    wavFile.close();
}
*/
