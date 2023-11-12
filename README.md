# Bibliothèque c++ utilisée :
SFML <br />
libnyquist : https://github.com/ddiakopoulos/libnyquist <br />
dans libnyquist :  <br />
    1. mkdir build <br />
    2. cd build <br />
    3. cmake .. <br />
    4. cmake --build . --target install <br />

# Compiler : 
- modifier PATH_TO dans Makefile avec le chemin vers la bibliothèque libnyquist <br />
- make <br />
- pour effacer *.o : make clean

# Executer :
./convert --OPTIONS file1 (file2) <br />
OPTIONS : <br />
--read file.mid <br />
--wav file1.mid file2.wav <br />
--midi file1.wav file2.mid <br />
--compare file1.mid file2.mid <br />

# Optimisation des resultats
Modifications de WIN_SIZE, HOP_SIZE et ENERGY_THRESHOLD pour de meilleurs resultats <br />
WIN_SIZE fenetre de taille de puissance de 2 (entre 1024 et 4096) <br />
HOP_SIZE fenetre de chevauchement : 25% ou 50% ou 75% de WIN_SIZE <br />
fichier gamme do : WIN_SIZE = 4096; HOP_SIZE = 2048; ENERGY_THRESHOLD = 200; <br />
fichier gamme do : WIN_SIZE = 4096; HOP_SIZE = 3072; ENERGY_THRESHOLD = 200; <br />
fichier gamme do : WIN_SIZE = 4096; HOP_SIZE = 2048; ENERGY_THRESHOLD = 50; <br />
 <br />
TRAN Marilyn <br />
VIGNESWARAN Tharsiya <br />

