#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "./headers/comandos.h"
#include "./headers/registros.h"
#include "./headers/resultados.h"

std::vector<long int> busca_sequencial(const std::string& tabela, const std::string& modo, const std::string& campo, const std::string& chave)
{
    Metadado mtd(tabela);

    char atingiu_max = 0;
    std::vector<long int> resultados;
    std::vector<Reg> campos;
    std::string linha;

    std::ifstream arquivo("./tabelas/" + tabela + ".dat");
    if (!arquivo.is_open())
    {
        std::cout << "Arquivo '" << tabela << "' não encontrado\n";
        EB();
    }

    // Armazena posição do registro e o lê
    long int pos = arquivo.tellg();
    getline(arquivo, linha);

    while (!atingiu_max && !arquivo.eof()) 
    {
        if (linha.find('#') == std::string::npos)
        {
            Registro reg(mtd, linha);
            campos = reg.lista_campos();

            for (int i = 0; i < campos.size(); i++)
            {
    
                if (campos.at(i).nome_campo == campo)
                {
                    if (campos.at(i).valor == chave) 
                    {
                        resultados.push_back(pos);
                        if (modo == "U")
                            atingiu_max = 1;
                    }
                }
            }
        }
        
        pos = arquivo.tellg();
        getline(arquivo, linha);
    }

    arquivo.close();
    return resultados;
}
