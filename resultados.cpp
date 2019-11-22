#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "./headers/comandos.h"
#include "./headers/registros.h"
#include "./headers/resultados.h"

std::vector<long> busca_sequencial(const Metadado& mtd, const std::string& tabela, const std::string& modo, const std::string& campo, const std::string& chave)
{
    char atingiu_max = 0;
    int indice = -1;
    std::vector<long> resultados;
    std::vector<Reg> registro;
    std::vector<Campo> campos = mtd.get_campos();
    std::string linha;

    std::ifstream arquivo("./tabelas/" + tabela + ".dat");
    if (!arquivo.is_open())
    {
        std::cout << "Arquivo '" << tabela << "' não encontrado\n";
        EB();
    }

    // Encontrando campo
    for (int i = 0; i < campos.size(); i++)
    {
        if (campos.at(i).nome == campo)
            indice = i;
    }
    if (indice == -1)
    {
        std::cout << "Campo não encontrado\n";
        EB();
    }

    // Armazena posição do registro e o lê
    long pos = arquivo.tellg();
    getline(arquivo, linha);

    while (!atingiu_max && !arquivo.eof()) 
    {
        // Verificando se o campo foi deletado
        if (linha.find(":") == std::string::npos)
        {
            linha = linha.substr(0, linha.find("#"));
            Registro reg(mtd, linha);
            registro = reg.lista_campos();

            if (registro.at(indice).tipo == "STR")
            {
                // Busca em string considera chave como parte do conteúdo
                if (registro.at(indice).valor.find(chave) != std::string::npos) 
                {
                    resultados.push_back(pos);
                    if (modo == "U")
                        atingiu_max = 1;
                }
            }
            else
            {
                if (registro.at(indice).valor == chave)
                {
                    resultados.push_back(pos);
                    if (modo == "U")
                        atingiu_max = 1;
                }
            }
        }
        
        pos = arquivo.tellg();
        getline(arquivo, linha);
    }

    arquivo.close();
    return resultados;
}
