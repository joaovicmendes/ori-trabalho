#ifndef REGISTROS_H
#define REGISTROS_H

#include <vector>
#include "metadados.h"

// @cleanup: criar tratamento especial para arquivo binário

typedef struct reg
{
    std::string       tipo;
    std::string nome_campo;
    std::string      valor;
} Reg;

class Registro
{
    public:
    // Construtor para recortar um registro, recebe uma lista de valores 
    // separada por ;
    Registro(const Metadado& mtd, const std::string& registro);

    // Construtor para criar um novo registro, recebe uma lista de valores
    // separada por ;
    Registro(const Metadado& mtd, const std::string& registro, bool novo);

    // Retorna this->campos
    std::vector<Reg> lista_campos();

    // Adiciona ao fim do arquivo de registros da this->tabela
    void append();

    // Adiciona a pos do arquivo de registros da this->tabela
    void insert(long pos);

    // Retorna o tamanho dos valores do registro (inclui os ;)
    long length();

    // Imprime conteúdos do registro
    void print();

    private:
    std::string      tabela; // De qual tabela o metadado é referente
    std::vector<Reg> campos; // Campos da tabela
};

#endif
