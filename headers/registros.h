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
    // Construtor para recuperar o registro existente na pos da tabela
    Registro(const Metadado& mtd, long int pos);

    // Construtor para criar um novo registro, recebe uma lisat de valores 
    // separada por ;
    Registro(const Metadado& mtd, const std::string& registro);
    
    // Adiciona ao fim do arquivo de registros da this->tabela
    void append();

    // Imprime conteúdos do registro
    void print();

    private:
    char             valido; // Se o registro é válido
    int                 tam; // Tamanho do registro em bytes
    std::string      tabela; // De qual tabela o metadado é referente
    std::vector<Reg> campos; // Campos da tabela
};

#endif
