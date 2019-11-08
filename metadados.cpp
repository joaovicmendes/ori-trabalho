#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/metadados.h"

Metadado::Metadado(const std::string& tabela)
{
    char *buffer = NULL;
    int buff_sz;
    int qtd_campos;
    Campo c;

    std::string path("./metadados/" + tabela + ".dat");
    FILE *arquivo = fopen_safe(path.c_str(), "rb");
    
    // Lendo tabela
    fread(&buff_sz, sizeof(buff_sz), 1, arquivo);
    // Alocando buffer e lendo tabela
    buffer = (char *) malloc_safe(buff_sz + 1);
    fread(&buffer, sizeof(char), buff_sz, arquivo);
    buffer[buff_sz] = '\0';

    this->tabela.assign(buffer);
    free(buffer);

    // Lendo campos
    fread(&qtd_campos, sizeof(this->n), 1, arquivo);
    for (int i = 0; i < qtd_campos; i++)
    {
        char modo = 'N';

        // Tamanho do tipo
        fread(&buff_sz, sizeof(buff_sz), 1, arquivo);
        // Tipo
        buffer = (char *) malloc_safe(buff_sz + 1);
        fread(&buffer, sizeof(char), buff_sz, arquivo);
        buffer[buff_sz] = '\0';
        c.tipo.assign(buffer);
        free(buffer);

        // Tamanho do nome
        fread(&buff_sz, sizeof(buff_sz), 1, arquivo);
        // Nome
        buffer = (char *) malloc_safe(buff_sz + 1);
        fread(&buffer, sizeof(char), buff_sz, arquivo);
        buffer[buff_sz] = '\0';
        c.nome.assign(buffer);
        free(buffer);

        this->add_campo(c);

        // Tipo de indice
        fwrite(&modo, sizeof(char), 1, arquivo);
        this->set_indice(c.nome, modo);
    }

    // Lendo lista de removidos
    fwrite(&buff_sz, sizeof(buff_sz), 1, arquivo);
    this->num_removidos = buff_sz;
    // Escrevendo n tipos, n campos e seus tamanhos antes
    for (int i = 0; i < this->num_removidos; i++)
    {
        long int rm;
        fread(&rm, sizeof(long int), 1, arquivo);
        this->removidos.push_back(rm);
    }

    fclose(arquivo);
}

void Metadado::set_tabela(const std::string& tabela)
{
    this->tabela = tabela;
}

int Metadado::num_campos()
{
    return this->n;
}

void Metadado::add_campo(Campo c)
{
    this->n++;
    this->campos.push_back(c);
    this->indice.push_back('N');
}

void Metadado::set_indice(const std::string& nome_campo, const char tipo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->indice.size() != this->campos.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    if (tipo == 'A' || tipo == 'N' || tipo == 'H')
    {
        for (int i = 0; i < this->indice.size(); i++)
        {
            if (this->campos.at(i).nome == nome_campo) 
                this->indice.at(i) = tipo;
        }
    } 
}

char Metadado::indice_em(std::string campo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->indice.size() != this->campos.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->indice.size(); i++)
    {
        if (this->campos.at(i).nome == campo) 
            return this->indice.at(i);
    }
    
    // Se campo não encontrado
    return false;
}

bool Metadado::hash_em(std::string campo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->indice.size() != this->campos.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->indice.size(); i++)
    {
        if (this->campos.at(i).nome == campo) 
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
    if (this->indice.size() != this->campos.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->indice.size(); i++)
    {
        if (this->campos.at(i).nome == campo) 
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

void Metadado::save()
{
    int buff_sz;
    std::string path("./metadados/" + this->tabela + ".dat");
    FILE *arquivo = fopen_safe(path.c_str(), "wb");
    
    // Escrevendo tamanho de this->tabela
    buff_sz = this->tabela.length();
    fwrite(&buff_sz, sizeof(buff_sz), 1, arquivo);
    // Escrevendo this->tabela
    fwrite(&(this->tabela), sizeof(char), buff_sz, arquivo);

    // Escrevendo número de campos
    fwrite(&(this->n), sizeof(this->n), 1, arquivo);
    // Escrevendo n tipos, n campos e seus tamanhos antes
    for (int i = 0; i < this->n; i++)
    {
        // Tamanho do tipo
        buff_sz = this->campos.at(i).tipo.length();
        fwrite(&buff_sz, sizeof(buff_sz), 1, arquivo);
        // Tipo
        fwrite(&(this->campos.at(i).tipo), sizeof(char), buff_sz, arquivo);

        // Tamanho do nome
        buff_sz = this->campos.at(i).nome.length();
        fwrite(&buff_sz, sizeof(buff_sz), 1, arquivo);
        // Nome
        fwrite(&(this->campos.at(i).nome), sizeof(char), buff_sz, arquivo);

        // Tipo de indice
        fwrite(&(this->indice.at(i)), sizeof(char), 1, arquivo);
    }

    // Escrevendo número de itens da lista de removidos
    fwrite(&(this->num_removidos), sizeof(this->num_removidos), 1, arquivo);
    // Escrevendo n tipos, n campos e seus tamanhos antes
    for (int i = 0; i < this->num_removidos; i++)
    {
        // Lista de removidos
        fwrite(&(this->removidos.at(i)), sizeof(long int), 1, arquivo);
    }

    fclose(arquivo);
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
        buffer = (char *) malloc_safe(buff_sz + 1);
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
    fwrite(&tabela, sizeof(char), buff_sz, arquivo);

    fclose(arquivo);
}

void remove_tabela(const std::string& tabela)
{
    char *buffer = NULL;
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
            buffer = (char *) malloc_safe(buff_sz + 1);
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
