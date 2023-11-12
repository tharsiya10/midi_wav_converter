all:
	g++ -Wall -c -g -I /home/tharsi/Documents/libnyquist/include/libnyquist/ Main.cpp compare_midi.cpp write_wav.cpp write_midi.cpp monophonic_detection.cpp yin.cpp read_midi/*.cpp util.cpp
	g++ *.o -o convert -llibnyquist -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

clean :
	rm *.o convert
