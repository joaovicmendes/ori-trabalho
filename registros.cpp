#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"
#include "./headers/metadados.h"
#include "./headers/registros.h"
#include "./headers/reaproveitamento.h"

Registro::Registro(const Metadado& mtd, const std::string& registro)
{
    this->tabela = mtd.get_tabela();
    std::vector<Campo> campos = mtd.get_campos();

    // Recortando lista de registros
    std::vector<std::string> vec = str_tokenize(registro, ';');
    if (vec.size() != campos.size())
        erro_sintaxe(2);

    for (int i = 0; i < vec.size(); i++)
    {
        if (campos.at(i).tipo == "INT")
        {
            try 
            {
                stol(vec.at(i));
            }
            catch (...)
            {
                std::cout << "Inserção inválida: valor não numérico em campo INT\n";
                EB();
            }
        }
        else if (campos.at(i).tipo == "FLT")
        {
            try
            {
                stof(vec.at(i));
            }
            catch (...)
            {
                std::cout << "Inserção inválida: valor não numérico em campo FLT\n";
                EB();
            }
        }

        Reg r;
        r.tipo = campos.at(i).tipo;
        r.nome_campo = campos.at(i).nome;
        r.valor = vec.at(i);

        this->campos.push_back(r);
    }
}

Registro::Registro(const Metadado& mtd, const std::string& registro, bool novo)
{
    this->tabela = mtd.get_tabela();
    std::vector<Campo> campos = mtd.get_campos();

    // Recortando lista de registros
    std::vector<std::string> vec = str_tokenize(registro, ';');
    if (vec.size() != campos.size())
        erro_sintaxe(2);

    for (int i = 0; i < vec.size(); i++)
    {
        std::string valor = vec.at(i);

        if (campos.at(i).tipo == "INT")
        {
            try 
            {
                stol(vec.at(i));
            }
            catch (...)
            {
                std::cout << "Inserção inválida: valor não numérico em campo INT\n";
                EB();
            }
        }
        else if (campos.at(i).tipo == "FLT")
        {
            try
            {
                stof(vec.at(i));
            }
            catch (...)
            {
                std::cout << "Inserção inválida: valor não numérico em campo FLT\n";
                EB();
            }
        }
        else if (campos.at(i).tipo == "BIN")
        {
            char c;
            std::ifstream arquivo(valor);
            if (!arquivo.is_open())
            {
                std::cout << "Arquivo BIN '" << valor << "' não encontrado\n";
                EB();
            }

            valor = "";

            arquivo.get(c);
            while (!arquivo.eof())
            {
                switch (c)
                {
                    case ':':
                        valor += "<t0k3n0x00>";
                        break;
                    case ';':
                        valor += "<t0k3n0x01>";
                        break;
                    case '#':
                        valor += "<t0k3n0x02>";
                        break;
                    case '\n':
                        valor += "<t0k3n0x03>";
                        break;
                    default:
                        valor += c;
                        break;
                }

                arquivo.get(c);
            }
        }

        Reg r;
        r.tipo = campos.at(i).tipo;
        r.nome_campo = campos.at(i).nome;
        r.valor = valor;

        this->campos.push_back(r);
    }
}

void Registro::append()
{
    std::string registro;
    std::string path("./tabelas/" + this->tabela + ".dat");
    std::ofstream arquivo(path, std::ios::out | std::ios::ate | std::ios::app);

    if (!arquivo.is_open())
    {
        std::cout << "Arquivo '" << path << "' não encontrado\n";
        EB();
    }

    for (int i = 0; i < this->campos.size(); i++)
    {
        registro += this->campos.at(i).valor;
        if (i + 1 != this->campos.size())
            registro += ";";
    }
    while (registro.size() < MIN_REG_SZ)
        registro += "#";

    arquivo << registro;
    arquivo << "\n";

    arquivo.close();
}

void Registro::insert(long pos)
{
    std::string registro;
    std::string path("./tabelas/" + this->tabela + ".dat");
    std::fstream arquivo(path);

    if (!arquivo.is_open())
    {
        std::cout << "Arquivo '" << path << "' não encontrado\n";
        EB();
    }

    arquivo.seekg(pos);

    for (int i = 0; i < this->campos.size(); i++)
    {
        registro += this->campos.at(i).valor;
        if (i + 1 != this->campos.size())
            registro += ";";
    }
    while (registro.size() < MIN_REG_SZ)
        registro += "#";

    arquivo << registro;

    arquivo.close();
}

long Registro::length()
{
    long sz = 0;
    for (int i = 0; i < this->campos.size(); i++)
        sz += this->campos.at(i).valor.length();
    sz += (this->campos.size() -  1);

    if (MIN_REG_SZ > sz)
        sz = MIN_REG_SZ;

    return sz;
}

std::vector<Reg> Registro::lista_campos()
{
    return this->campos;
}


void Registro::print()
{
    int largest_name = 0, largest_val = 0;

    for (int i = 0; i < this->campos.size(); i++)
    {
        if (this->campos.at(i).nome_campo.size() > largest_name)
            largest_name = this->campos.at(i).nome_campo.size();
        if (this->campos.at(i).tipo != "BIN") 
        {
            if (this->campos.at(i).valor.size() > largest_val)
                largest_val = this->campos.at(i).valor.size();
        }
    }

    for (int i = 0; i < this->campos.size(); i++)
    {
        std::cout << "  (" + this->campos.at(i).tipo + ") ";
        std::cout << std::left << std::setw(largest_name) << this->campos.at(i).nome_campo << " ";
        
        if (this->campos.at(i).tipo != "BIN")
            std::cout << std::left << std::setw(largest_val + 2) << "'" + this->campos.at(i).valor + "'";
        else
        {
            std::string aux;

            for (int j = 0; j < this->campos.at(i).valor.size(); j++)
            {
                // Se ainda há espaço pro token
                if (j + 10 < this->campos.at(i).valor.size())
                {
                    // Se encontrou parte inicial do token
                    if (this->campos.at(i).valor.substr(j, 8) == "<t0k3n0x")
                    {
                        if (this->campos.at(i).valor.substr(j+8, 3) == "00>")
                            aux += ":", j += 10;
                        else if (this->campos.at(i).valor.substr(j+8, 3) == "01>")
                            aux += ";", j += 10;
                        else if (this->campos.at(i).valor.substr(j+8, 3) == "02>")
                            aux += "#", j += 10;
                        else if (this->campos.at(i).valor.substr(j+8, 3) == "03>")
                            aux += "\n", j += 10;
                        else
                            aux += this->campos.at(i).valor.at(j);
                    }
                    else
                        aux += this->campos.at(i).valor.at(j);
                }
                else
                    aux += this->campos.at(i).valor.at(j);
            }

            std::cout << "\n\"" << aux << "\"";
        }
        std::cout << "\n";

    }
}
