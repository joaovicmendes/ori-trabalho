#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"
#include "./headers/metadados.h"
#include "./headers/registros.h"

Registro::Registro(const Metadado& mtd, long int pos)
{
}

Registro::Registro(const Metadado& mtd, const std::string& registro)
{
    this->valido = 1;
    this->tabela = mtd.get_tabela();
    std::vector<Campo> campos = mtd.get_campos();

    // Recortando lista de registros
    std::vector<std::string> vec = str_tokenize(registro, ';');
    if (vec.size() != campos.size())
        erro_sintaxe(2);

    for (int i = 0; i < vec.size(); i++)
    {
        Reg r;
        r.tipo = campos.at(i).tipo;
        r.nome_campo = campos.at(i).nome;
        r.valor = vec.at(i);

        this->campos.push_back(r);
    }

    this->tam = 0;
}

void Registro::save(long int pos)
{
}

void Registro::print()
{
    int largest_name = 0, largest_val = 0;

    for (int i = 0; i < this->campos.size(); i++)
    {
        if (this->campos.at(i).nome_campo.size() > largest_name)
            largest_name = this->campos.at(i).nome_campo.size();
        if (this->campos.at(i).valor.size() > largest_val)
            largest_val = this->campos.at(i).valor.size();
    }

    for (int i = 0; i < this->campos.size(); i++)
    {
        std::cout << "  (" + this->campos.at(i).tipo + ") ";
        std::cout << std::left << std::setw(largest_name) << this->campos.at(i).nome_campo << " ";
        std::cout << std::left << std::setw(largest_val + 2) << "'" + this->campos.at(i).valor + "'" << "\n";
    }
}
