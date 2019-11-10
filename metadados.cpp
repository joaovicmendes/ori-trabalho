#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdio>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/metadados.h"

Metadado::Metadado() {}


Metadado::Metadado(const std::string& tabela)
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existente na base de dados\n";
        EB();
    }

    char *buffer;
    int n;
    int buff_sz;
    std::string path("./metadados/" + tabela + ".dat");
    FILE *arquivo = fopen_safe(path.c_str(), "rb");
    
    fread(&buff_sz, sizeof(buff_sz), 1, arquivo);
    buffer = (char *) malloc_safe( (buff_sz + 1) * (sizeof(char)) );
    fread(buffer, sizeof(char), buff_sz, arquivo);
    buffer[buff_sz] = '\0';
    this->tabela.assign(buffer);
    free(buffer), buffer = NULL;

    fread(&n, sizeof(n), 1, arquivo);
    for (int i = 0; i < n; i++)
    {
        Campo c;

        // Tipo
        fread(&buff_sz, sizeof(buff_sz), 1, arquivo);
        buffer = (char *) malloc_safe( (buff_sz + 1) * (sizeof(char)) );
        buffer[buff_sz] = '\0';
        fread(buffer, sizeof(char), buff_sz, arquivo);
        c.tipo.assign(buffer);
        free(buffer), buffer = NULL;


        // Nome
        fread(&buff_sz, sizeof(buff_sz), 1, arquivo);
        buffer = (char *) malloc_safe( (buff_sz + 1) * (sizeof(char)) );
        buffer[buff_sz] = '\0';
        fread(buffer, sizeof(char), buff_sz, arquivo);
        c.nome.assign(buffer);
        free(buffer), buffer = NULL;


        // Indice
        fread(&c.indice, sizeof(char), 1, arquivo);
        this->add_campo(c);
    }

    fread(&n, sizeof(n), 1, arquivo);
    for (int i = 0; i < n; i++)
    {
        Removido r;
        fread(&r, sizeof(long int), 2, arquivo);

        this->add_removido(r);
    }

    fclose(arquivo);
}

int Metadado::num_campos() const
{ 
    return this->campos.size();
}

std::string Metadado::get_tabela() const
{
    return this->tabela;
}

std::vector<Campo> Metadado::get_campos() const
{
    return this->campos;
}

std::vector<Removido> Metadado::get_removidos() const
{
    return this->removidos;
}

void Metadado::set_tabela(const std::string& tabela)
{
    this->tabela = tabela;
}

void Metadado::add_campo(Campo c)
{
    this->campos.push_back(c);
}

void Metadado::add_removido(Removido r)
{
    this->removidos.push_back(r);
    std::sort(this->removidos.begin(), this->removidos.end(), std::greater<Removido>());
}

void Metadado::del_removido(long int indice)
{
    bool removeu = false;
    for (int i = 0; i < this->removidos.size() && !removeu; i++)
    {
        if (this->removidos.at(i).indice == indice) 
        {
            this->removidos.erase(this->removidos.begin() + i);
            removeu = true;
        }
    }
    std::sort(this->removidos.begin(), this->removidos.end(), std::greater<Removido>());
}

Removido Metadado::maior_removido() const
{
    return this->removidos.front();
}

void Metadado::set_indice(const std::string& nome_campo, const char indice)
{
    if (indice == 'A' || indice == 'N' || indice == 'H')
    {
        for (int i = 0; i < this->campos.size(); i++)
        {
            if (this->campos.at(i).nome == nome_campo) 
                this->campos.at(i).indice = indice;
        }
    } 
    else
    {
        std::cout << "Índice inexistente sendo adicionado\n";
        EB();
    }
}

char Metadado::indice_em(std::string campo) const
{
    for (int i = 0; i < this->campos.size(); i++)
    {
        if (this->campos.at(i).nome == campo) 
            return this->campos.at(i).indice;
    }

    // Se campo não encontrado
    return '\0';
}

bool Metadado::hash_em(std::string campo) const
{
    for (int i = 0; i < this->campos.size(); i++)
    {
        if (this->campos.at(i).nome == campo) 
        {
            if (this->campos.at(i).indice == 'H')
                return true;
            else
                return false;
        }
    }

    // Se campo não encontrado
    return false;
}

bool Metadado::arvore_em(std::string campo) const
{
    for (int i = 0; i < this->campos.size(); i++)
    {
        if (this->campos.at(i).nome == campo) 
        {
            if (this->campos.at(i).indice == 'A')
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
    char indice;
    int buff_sz;
    std::string path("./metadados/" + this->tabela + ".dat");
    FILE *arquivo = fopen_safe(path.c_str(), "wb");
    
    // Escrevendo tamanho de this->tabela
    buff_sz = this->tabela.length();
    fwrite(&buff_sz, sizeof(int), 1, arquivo);
    fwrite(this->tabela.c_str(), sizeof(char), buff_sz, arquivo);

    // Escrevendo número de campos
    int n = this->campos.size();
    fwrite(&n, sizeof(int), 1, arquivo);
    for (int i = 0; i < n; i++)
    {
        // Tipo
        buff_sz = this->campos.at(i).tipo.length();
        fwrite(&buff_sz, sizeof(int), 1, arquivo);
        fwrite(this->campos.at(i).tipo.c_str(), sizeof(char), buff_sz, arquivo);

        // Nome
        buff_sz = this->campos.at(i).nome.length();
        fwrite(&buff_sz, sizeof(int), 1, arquivo);
        fwrite(this->campos.at(i).nome.c_str(), sizeof(char), buff_sz, arquivo);

        // Indice
        indice = this->campos.at(i).indice;
        fwrite(&indice, sizeof(indice), 1, arquivo);
    }

    n = this->removidos.size();
    fwrite(&n, sizeof(int), 1, arquivo);
    for (int i = 0; i < n; i++)
    {
        Removido r = this->removidos.at(i);
        fwrite(&r, sizeof(long int), 2, arquivo);
    }

    fclose(arquivo);
}

void Metadado::print() const
{
    int largest = 0;
    for (int i = 0; i < this->campos.size(); i++)
        if (this->campos.at(i).nome.size() > largest)
            largest = this->campos.at(i).nome.size();

    std::cout << " " << this->tabela << "\n";
    for (int i = 0; i < this->campos.size(); i++)
    {
        std::cout << "  '" + this->campos.at(i).tipo + "':";
        std::cout << std::left << std::setw(largest + 2) << "'" + this->campos.at(i).nome + "'";
        std::cout << " (" << this->campos.at(i).indice << ")\n";
    }

    std::cout << "  Removidos:\n";
    for (int i = 0; i < this->removidos.size(); i++)
    {
        std::cout << "     " << this->removidos.at(i).indice << " (" << this->removidos.at(i).tamanho << ")\n";
    }
}


bool tem_tabela(const std::string& tabela)
{
    char *buffer = NULL;
    int buff_sz = 0;
    std::string tmp;

    FILE *arquivo = fopen_safe(SGBD_PATH, "ab+");
    rewind(arquivo);

    while (fread(&buff_sz, sizeof(buff_sz), 1, arquivo))
    {
        // Alocando buffer e lendo sequência
        buffer = (char *) malloc_safe(sizeof(char) * (buff_sz + 2));
        fread(buffer, sizeof(char), buff_sz, arquivo);
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

    fwrite(&buff_sz, sizeof(buff_sz), 1, arquivo);
    fwrite(tabela.c_str(), sizeof(char), buff_sz, arquivo);

    fclose(arquivo);
}

void remove_tabela(const std::string& tabela)
{
    char *buffer = NULL;
    int buff_sz;
    std::string tmp;
    FILE *arquivo = fopen_safe(SGBD_PATH, "rb+");

    while (fread(&buff_sz, sizeof(buff_sz), 1, arquivo))
    {
        buffer = (char *) malloc_safe(buff_sz + 1);
        buffer[buff_sz] = '\0';
        fread(buffer, sizeof(char), buff_sz, arquivo);

        tmp.assign(buffer);
        free(buffer);

        if (tmp != tabela)
            insere_tabela("./metadados/tmp", tabela);
    }
    fclose(arquivo);

    remove(SGBD_PATH);
    rename("./metadados/tmp", SGBD_PATH);
}

bool operator>(const Removido& a, const Removido& b)
{
    return (a.tamanho > b.tamanho);
}

bool operator<(const Removido& a, const Removido& b)
{
    return (a.tamanho < b.tamanho);
}
