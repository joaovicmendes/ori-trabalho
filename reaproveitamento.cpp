#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "./headers/metadados.h"
#include "./headers/reaproveitamento.h"

long int busca_removido(Metadado& mtd, long int tam)
{
    bool achou = false;
    long int prev = -1, prox, sz, aux;
    std::string input;
    std::fstream arquivo("./tabelas/" + mtd.get_tabela() + ".dat");
    
    aux = mtd.get_removido();
    while (aux != -1 && !achou)
    {
        arquivo.seekg(aux);
        getline(arquivo, input, ':');
        prox = std::stol(input);
        // @cleanup tratar exceção de conversão

        getline(arquivo, input, '#');
        sz = std::stol(input);

        if (sz >= tam)
        {
            achou = true;
            // Se o anterior for o valor nos metadados
            if (prev == -1)
                mtd.set_removido(prox);
            else
            {
                arquivo.seekg(prev);
                arquivo << std::setw(sizeof(long)) << prox;
            }
        }

        prev = aux;
        aux = prox;
    }
    arquivo.close();

    return prev;
}
