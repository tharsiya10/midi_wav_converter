#include "write_wav.h"
#include "write_midi.h"
#include "compare_midi.h"
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    if(argc < 1){
        cout << "Usage " << argv[0] << " --options (read/midi/wav/compare) <file...>" << endl;
        return 1;
    }
    string option = argv[1];
    if(option == "--read"){
        if(argc < 3){
            cout << "Usage " << argv[0] << "--read file.mid" << endl; 
            return 1;
        }
        Midi m1(argv[2]);
        cout << m1 << endl;
        return 0;
    }
    if(argc < 4){
        cout << "Usage " << argv[0] << "--options (midi/wav/compare) file1 file2" << endl; 
        return 1;
    }    
    if(option.compare("--midi") == 0){
        write_to_midi(argv[2], argv[3]);
    }
    
    else if(option == "--wav"){
        Midi m1(argv[2]);
        write_to_wav(&m1, argv[3]);
    }

    else if(option == "--compare") {
        if(argc < 4) {
            return 1;
        }
        Midi f1(argv[2]);
        Midi f2(argv[3]);
        get_similarity(&f1, &f2);
    }
    else {
        return 1;
    }

    return 0;
}