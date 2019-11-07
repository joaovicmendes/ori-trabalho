#include <iostream>
#include <string>
#include <cctype>
#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"

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

