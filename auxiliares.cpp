#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"
#include "./headers/metadados.h"

bool tem_espaco(const std::string& texto) 
{
    for (int i = 0; i <= texto.length(); i++)
    {
            if (texto[i] == ' ') 
                return true;
    }
    return false;
}

bool tipo_valido(const std::string& texto) 
{
    return (texto == "INT" || texto == "FLT" || texto == "STR" || texto == "BIN");
}

void to_upper(std::string& texto)
{
    for (int i = 0; i < texto.length(); i++)
        texto.at(i) = toupper(texto.at(i));
}

int qtd_campos(const std::string& campos) 
{
    int aux1 = 0;
    int aux2 = 0;

    for (int i = 0; i < campos.length(); i++)
    {
        if (campos[i] == ';')
            aux1++;
        else if (campos[i] == ':')
            aux2++;
    }

    if (++aux1 != aux2)
        erro_sintaxe(2);
    return aux1;
}

int qtd_registros(const std::string& campos) 
{
    int aux = 0;

    for (int i = 0; i < campos.length(); i++)
    {
        if (campos[i] == ';')
            aux++;
    }
    return aux + 1;
}

void *malloc_safe(unsigned nbytes)
{
    void *p = malloc(nbytes);
    if (p == NULL)
    {
        std::cout << "Não foi possível alocar memória\n";
        EB();
    }

    return p;
}

FILE *fopen_safe(const char *path, const char *modo)
{
    FILE *arquivo = fopen(path, modo);
    if (arquivo == NULL)
    {
        std::cout << "Arquivo '" << path << "' não encontrado\n";
        EB();
    }

    return arquivo;
}

std::vector<std::string> str_tokenize(const std::string& texto, const char delim)
{
    std::vector<std::string> tokens;
    std::string tmp;
    std::stringstream ss(texto);

    while (getline(ss, tmp, delim))
        tokens.push_back(tmp);

    return tokens;
}

long reg_count(const std::string& tabela)
{
    std::fstream arquivo("./tabelas/" + tabela + ".dat");    
    char c = 0;
    int i  = 0;

    while (!arquivo.eof())
    {
        arquivo.get(c);
        if (c == '\n')
            i++;
    }

    return i;
}
