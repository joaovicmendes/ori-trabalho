#ifndef METADADOS_H
#define METADADOS_H

#include <vector>

#define SGBD_PATH "./metadados/sgdb"

typedef struct campo
{
    std::string tipo;
    std::string nome;
} Campo;

typedef struct registro
{
    std::vector<std::string> tipo;
    std::vector<std::string> nome;
} Registro;

class Metadado
{
    public:
    // Construtor padrão, para adicionar uma nova tabela
    Metadado();

    // Construtor para recuperar dados de uma nova tabela
    Metadado(const std::string& tabela);

    // Define à qual tabela pertence
    void set_tabela();

    // Número de campos da tabela em questão
    int num_campos();

    // Definindo número de campos da tabela em questão
    void set_num_campos(int n);

    // Adiciona um campo e um índice N à tabela
    void add_campo(const Campo& c);

    // Altera o índice de dado campo
    void set_indice(const std::string& nome_campo);

    // Retorna qual o índice em campo (N: nenhum; A:árvore; H:hash)
    char indice_de(std::string campo);

    // Se existe índice hash em campo
    bool hash_em(std::string campo);

    // Se existe índice árvore em campo
    bool arvore_em(std::string campo);

    private:
    std::string              tabela; // De qual tabela o metadado é referente
    int                           n; // Número de campos da tabela
    std::vector<Campo>       campos; // Campos da tabela
    std::vector<char>        indice; // Se o n-ésimo elemento tem indice (N: nenhum; A:árvore; H:hash)
    int               num_removidos; // Número de itens na lista de removidos
    std::vector<long int> removidos; // Lista de posições removidas
};

// Recebe de uma tabela, e verifica se ela esta presente na base.
// @param std::string: nome da tabela a ser verificada
// @ret          bool: true para sucesso, false para falha
bool tem_tabela(const std::string& tabela);

// Recebe o nome de uma tabela, e adiciona ela no fim do arquivo da base.
// Escreve primeiro o tamanho do nome da tabela (sizeof int) e depois o nome em
// si.
// @param std::string: nome da tabela a ser adicionada
void insere_tabela(const char* path, const std::string& tabela);

// Recebe o nome de uma tabela, e a remove do arquivo de metadados do sistema.
// @param std::string: nome da tabela a ser removida
void remove_tabela(const std::string& tabela);

#endif
