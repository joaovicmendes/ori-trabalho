# Tirado de stackoverflow: https://stackoverflow.com/questions/41205042/c-makefile-headers-and-cpp
# Com adições das aulas de AED1 do Prof. Mário 

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic

OBJECTS = main.o interpretador.o comandos.o auxiliares.o

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJECTS): ./headers/interpretador.h ./headers/comandos.h ./headers/auxiliares.h

make clean:
	rm *.o
	rm ./metadados/*.dat
	rm ./metadados/SGDB/*.dat
	rm ./tabelas/*.dat
