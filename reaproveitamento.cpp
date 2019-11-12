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
            // Fragmentação interna
            if (sz - tam < 25)
            {
                arquivo.seekg(aux + tam);
                for (int i = 0; i < sz - tam; i++)
                    arquivo << "#";
            }
            // Fragmentação externa
            else
            {
                arquivo.seekg(aux + tam + 1);
                arquivo << "\n";
                int indice_frag = arquivo.tellg();

                getline(arquivo, input);
                int tam_frag = ((long int)arquivo.tellg()) - indice_frag - 1;


                // Cria novo espaço alocado
                arquivo.seekg(indice_frag);
                arquivo << std::setw(sizeof(long)) << prox;
                arquivo << ":";
                arquivo << std::setw(sizeof(long)) << tam_frag;
                arquivo << "#";

                // Atualiza aux para apontar para o novo espaço alocado
                prox = indice_frag;
            }

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
