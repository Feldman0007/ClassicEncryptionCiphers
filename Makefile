all:	cipher

cipher:	cipher.o Playfair.o Caesar.o RowTransposition.o Vigenre.o Railfence.o
	g++ cipher.o Playfair.o Caesar.o RowTransposition.o Vigenre.o Railfence.o -o cipher

cipher.o:	cipher.cpp
	g++ -g -c cipher.cpp 

Playfair.o:	Playfair.cpp Playfair.h CipherInterface.h
	g++ -g -c Playfair.cpp

Caesar.o: Caesar.cpp Caesar.h  CipherInterface.h
	g++ -g -c Caesar.cpp

RowTransposition.o:	RowTransposition.cpp RowTransposition.h CipherInterface.h
	g++ -g -c RowTransposition.cpp

Vigenre.o: Vigenre.cpp Vigenre.h  CipherInterface.h
	g++ -g -c Vigenre.cpp

Railfence.o: Railfence.cpp Railfence.h  CipherInterface.h
	g++ -g -c Railfence.cpp

clean:
	rm -rf *.o cipher