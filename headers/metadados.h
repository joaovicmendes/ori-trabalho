#ifndef METADADOS_H
#define METADADOS_H

#include <vector>

#define SGBD_PATH "./metadados/sgdb"

typedef struct campo
{
    std::string tipo;
    std::string nome;
    char indice;     // (N: nenhum; A:árvore; H:hash)
} Campo;

typedef struct removido
{
    long int indice;  // Índice inicial do espeço removido
    long int tamanho; // Tamanho do espaço removido
} Removido;

class Metadado
{
    public:
    // Construtor padrão, para adicionar uma nova tabela
    Metadado();

    // Construtor para recuperar dados de uma nova tabela
    Metadado(const std::string& tabela);

    // Número de campos da tabela em questão
    int num_campos() const;

    // Retorna string com o nome da tabela referente à esses metadados
    std::string get_tabela() const;

    // Retorna um vector dos campos dessa tabela
    std::vector<Campo> get_campos() const;

    // Retorna um vector dos removidos dessa tabela
    std::vector<Removido> get_removidos() const;

    // Define à qual tabela pertence
    void set_tabela(const std::string& tabela);

    // Adiciona um campo e um índice N à tabela
    void add_campo(Campo c);

    // Adiciona uma posição removida da tabela
    void add_removido(Removido r);

    // Retira uma posição removida da tabela
    void del_removido(long int indice);

    // Retorna o maior espaço removido da tabela
    Removido maior_removido() const;

    // Altera o índice de dado campo
    void set_indice(const std::string& nome_campo, const char indice);

    // Retorna qual o índice em campo (N: nenhum; A:árvore; H:hash)
    char indice_em(std::string campo) const;

    // Se existe índice hash em campo
    bool hash_em(std::string campo) const;

    // Se existe índice árvore em campo
    bool arvore_em(std::string campo) const;

    // Salva no arquivo
    void save();

    // Imprime metadados
    void print() const;

    private:
    std::string              tabela; // De qual tabela o metadado é referente
    std::vector<Campo>       campos; // Campos da tabela
    std::vector<Removido> removidos; // Lista de espaços removidos na tabela
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

bool operator>(const Removido& a, const Removido& b);

bool operator<(const Removido& a, const Removido& b);

#endif
