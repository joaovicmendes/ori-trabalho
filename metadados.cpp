#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/metadados.h"

Metadado::Metadado(const std::string& tabela)
{
    // Implementar abertura da tabela
}

void Metadado::set_tabela(const std::string& tabela)
{
    if (this->tabela.empty())
        this->tabela = tabela;
}

int Metadado::num_campos()
{
    return this->n;
}

void Metadado::set_num_campos(int n)
{
    if (n > 0)
        this->n = n;
}

void Metadado::add_campo(const Campo& c)
{
    this->campos.push_back(c);
    this->indice.push_back('N');
}

void Metadado::set_indice(const std::string& nome_campo, const char tipo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->indice.size() != this->campo.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    if (tipo == 'A' || tipo == 'N' || tipo == 'H')
    {
        for (int i = 0; i < this->indice.size(); i++)
        {
            if (this->campos.at(i) == nome_campo) 
                this->indice.at(i) = modo;
        }
    } 
}

char Metadado::indice_em(std::string campo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->indice.size() != this->campo.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->indice.size(); i++)
    {
        if (this->campos.at(i) == campo) 
            return this->indice.at(i);
    }
    
    // Se campo não encontrado
    return false;
}

bool Metadado::hash_em(std::string campo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->indice.size() != this->campo.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->indice.size(); i++)
    {
        if (this->campos.at(i) == campo) 
        {
            if (this->indice.at(i) == 'H')
                return true;
            else
                return false;
        }
    }
    
    // Se campo não encontrado
    return false;
}

bool Metadado::arvore_em(std::string campo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->indice.size() != this->campo.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->indice.size(); i++)
    {
        if (this->campos.at(i) == campo) 
        {
            if (this->indice.at(i) == 'A')
                return true;
            else
                return false;
        }
    }

    // Se campo não encontrado
    return false;
}

bool tem_tabela(const std::string& tabela)
{
    char *buffer = NULL;
    int buff_sz;
    std::string tmp;

    FILE *arquivo = fopen_safe(SGBD_PATH, "ab+");

    while (!feof(arquivo))
    {
        // Lendo tamanho da sequência a seguir
        fread(&buff_sz, sizeof(buff_sz), 1, arquivo);

        // Alocando buffer e lendo sequência
        buffer = malloc_safe(buff_sz + 1);
        fread(&buffer, sizeof(char), buff_sz, arquivo);
        buffer[buff_sz] = '\0';

        tmp.assign(buffer);
        free(buffer);

        if (tmp == tabela)
        {
            fclose(arquivo);
            return true;
        }
    }

    fclose(arquivo);
    return false;
}

void insere_tabela(const char* path, const std::string& tabela)
{
    int buff_sz = tabela.length();
    FILE *arquivo = fopen_safe(path, "ab+");

    // Escrevendo tamanho da sequência
    fwrite(&buff_sz, sizeof(buff_sz), 1, arquivo);
    // Escrevendo nome da tabela (não inclui \0)
    fwrite(&(tabela.c_str()), sizeof(char), buff_sz, arquivo);

    fclose(arquivo);
}

void remove_tabela(const std::string& tabela)
{
    char *buffer = NULL
    int buff_sz;
    std::string tmp;
    FILE *arquivo = fopen_safe(SGBD_PATH, "ab+");

    if (tem_tabela(tabela))
    {
        while (!feof(arquivo))
        {
           // Lendo tamanho da sequência a seguir
            fread(&buff_sz, sizeof(buff_sz), 1, arquivo);

            // Alocando buffer e lendo sequência
            buffer = malloc_safe(buff_sz + 1);
            fread(&buffer, sizeof(char), buff_sz, arquivo);
            buffer[buff_sz] = '\0';

            tmp.assign(buffer);
            free(buffer);

            if (tmp != tabela)
                insere_tabela("./metadados/tmp", "ab+");
        }
        remove(SGBD_PATH);
        rename("./metadados/tmp", SGBD_PATH);
    }

    fclose(arquivo);
}
