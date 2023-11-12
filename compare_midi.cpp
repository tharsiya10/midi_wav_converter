#include "compare_midi.h"

void get_midi_notes(vector<uint8_t> &notes, vector<int64_t> &debut, vector<int64_t> &fin, vector<MidiEvent*> &events){
    for(auto elt : events){
        if(elt->getEventType() == NOTE_ON && elt->getSecParam() > 0){
            notes.push_back(elt->getFirstParam());
            debut.push_back(elt->getVTime());
        }
        if((elt->getEventType() == NOTE_ON && elt->getSecParam() == 0) || 
        elt->getEventLength() == NOTE_OFF){
            fin.push_back(elt->getVTime());
        }
    }

}

/*
levenshtein distance : calculate number of edits, substitution, addition needed to go from v1 to v2
personnalized : can change tolerance
*/
template<typename T>
int levenshtein(vector<T> &v1, vector<T> &v2, int tolerance) {
    size_t size1 = v1.size();
    size_t size2 = v2.size();
    int matrix[size1+1][size2+1];
    if(size1 == 0){
        return (int) size2;
    }
    if(size2 == 0){
        return (int) size1;
    }

    for(size_t i = 0; i <= size1; i++){
        matrix[i][0] = i;
    }
    for(size_t j = 0; j <= size2; j++){
        matrix[0][j] = j;
    }

    for(size_t i = 1; i <=  size1; i++){
        for(size_t j = 1; j <= size2; j++){
            int cout;
            if(v1[i-1] >= v2[j-1] - tolerance || 
            v1[i-1] <= v2[j-1]+tolerance) {
                cout = 0;
            }
            else {
                cout = 1;
            }

            matrix[i][j] = min(
                min(matrix[i-1][j] + 1, matrix[i][j-1] + 1), 
                matrix[i-1][j-1] + cout
            );
        }
    }
    return matrix[size1][size2];
}


void get_similarity(Midi *m1, Midi *m2){
    vector<MidiEvent*> e1;
    vector<uint8_t> n1; vector<int64_t> d1; vector<int64_t> f1;
    get_midi_events(e1, m1);
    get_midi_notes(n1, d1, f1, e1);

    vector<MidiEvent*> e2;
    vector<uint8_t> n2; vector<int64_t> d2; vector<int64_t> f2;
    get_midi_events(e2, m2);
    get_midi_notes(n2, d2, f2, e2);

    int n_sim = levenshtein(n1, n2, PITCH_TOLERANCE);
    int d_sim = levenshtein(d1, d2, VTIME_TOLERANCE);
    int f_sim = levenshtein(f1, f2, VTIME_TOLERANCE);

    cout << "Distance de levenshtein sur les notes " << n_sim << "(/" << max(n1.size(), n2.size()) << ")" << endl;

    cout << "Distance de levenshtein sur temps de début " << d_sim << "(/" << max(d1.size(), d2.size()) << ")" << endl;
    cout << "Distance de levenshtein sur temps de fin " << f_sim << "(/" << max(f1.size(), f2.size()) << ")" << endl;

}