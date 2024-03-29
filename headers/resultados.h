#ifndef RESULTADOS_H
#define RESULTADOS_H

#include <vector>

// Os resultados são armazenados num std::map<tabela, lista de res>, que fica
// instânciado no arquivo comandos.cpp
//
// std::map<std::string, std::vector<long> > lista_resultados;

// Realiza busca sequencial em tabela por registros do campo que sejam iguais à chave
std::vector<long> busca_sequencial(const Metadado& mtd, const std::string& tabela, const std::string& modo, const std::string& campo, const std::string& chave);

#endif
