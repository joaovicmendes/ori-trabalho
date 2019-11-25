# Tirado de stackoverflow: https://stackoverflow.com/questions/41205042/c-makefile-headers-and-cpp
# Com adições das aulas de AED1 do Prof. Mário 

CXX = g++
CXXFLAGS = -std=c++11 #-Wall -pedantic -O3

OBJECTS = main.o interpretador.o comandos.o auxiliares.o metadados.o registros.o resultados.o reaproveitamento.o hash.o disktree.o

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	rm -f *.o ./metadados/sgdb ./metadados/*.dat ./tabelas/*.dat ./indices/*.dat ./indices_arvore/*.dat

interpretador.o: ./headers/interpretador.h
	$(CXX) $(CXXFLAGS) -c interpretador.cpp

$(OBJECTS): ./headers/interpretador.h ./headers/comandos.h ./headers/auxiliares.h ./headers/metadados.h ./headers/registros.h ./headers/resultados.h ./headers/reaproveitamento.h ./headers/hash.h ./headers/disktree.h

make clean:
	rm ./metadados/sgdb ./metadados/*.dat ./tabelas/*.dat ./indices/*.dat
