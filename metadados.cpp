#include <iostream>

#include "./headers/comandos.h"
#include "./headers/metadados.h"

Metadado::Metadado(const std::string& tabela)
{
    // Implementar abertura da tabela
}

int Metadado::num_campos()
{
    return this->num_campos;
}

char Metadado::indice_em(std::string campo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->tem_indice.size() != this->campo.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->tem_indice.size(); i++)
    {
        if (this->campos.at(i) == campo) 
            return this->tem_indice.at(i);
    }
    
    // Se campo não encontrado
    return false;
}

bool Metadado::hash_em(std::string campo)
{
    // Caso não tenha o mesmo número de elementos em ambas as listas
    if (this->tem_indice.size() != this->campo.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->tem_indice.size(); i++)
    {
        if (this->campos.at(i) == campo) 
        {
            if (this->tem_indice.at(i) == "H")
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
    if (this->tem_indice.size() != this->campo.size())
    {
        std::cout << "Inconsistência no tamanho da lista de campos e na lista de indices\n";
        EB();
    }

    for (int i = 0; i < this->tem_indice.size(); i++)
    {
        if (this->campos.at(i) == campo) 
        {
            if (this->tem_indice.at(i) == "A")
                return true;
            else
                return false;
        }
    }

    // Se campo não encontrado
    return false;
}
