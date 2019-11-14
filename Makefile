# Tirado de stackoverflow: https://stackoverflow.com/questions/41205042/c-makefile-headers-and-cpp
# Com adições das aulas de AED1 do Prof. Mário 

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic

OBJECTS = main.o interpretador.o comandos.o auxiliares.o metadados.o registros.o resultados.o reaproveitamento.o hash.o

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	rm *.o ./metadados/sgdb ./metadados/*.dat ./tabelas/*.dat

$(OBJECTS): ./headers/interpretador.h ./headers/comandos.h ./headers/auxiliares.h ./headers/metadados.h ./headers/registros.h ./headers/resultados.h ./headers/reaproveitamento.h ./headers/hash.h

make clean:
	rm ./metadados/sgdb ./metadados/*.dat ./tabelas/*.dat
