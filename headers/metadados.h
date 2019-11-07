#ifndef METADADOS_H
#define METADADOS_H

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
    Metadado(const std::string& tabela);

    // Número de campos da tabela em questão
    int num_campos();

    // Retorna qual o índice em campo (N: nenhum; A:árvore; H:hash)
    char indice_de(std::string campo);

    // Se existe índice hash em campo
    bool hash_em(std::string campo);

    // Se existe índice árvore em campo
    bool arvore_em(std::string campo);

    private:
    std::string           tabela; // De qual tabela o metadado é referente
    int               num_campos; // Número de campos da tabela
    std::vector<Campo>    campos; // Campos da tabela
    std::vector<char> tem_indice; // Se o n-ésimo elemento tem indice (N: nenhum; A:árvore; H:hash)
};

#endif
