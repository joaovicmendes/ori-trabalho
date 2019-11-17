#ifndef HASH_H
#define HASH_H

#include "metadados.h"

#define PAIR_SIZE (2 * sizeof(long))
#define BLOCK_CAP 128
#define BLOCK_SIZE (PAIR_SIZE * (BLOCK_CAP + 1))

typedef struct par
{
    long chave;
    long  cont;
} Par;

typedef struct bloco
{
    Par arr[BLOCK_CAP]; // Vetor de (chave, cont)
    long      num_elem; // Número de elementos no bloco
    long          prox; // Índice no arquivo para bloco extendido desse
} Bloco;

// Função que, dada uma quatidade total esperada de elementos, cria na memória
// secundária os blocos necessários para abriga-los no arquivo hash_path
void inicializa_hash(const std::string& hash_path, long num_elem);

// Insere um par p na hash do arquivo hash_path
void insere_hash(const std::string& hash_path, Par p);

// Retorna todos os pares que contém a chave na hash de hash_path
std::vector<long> busca_hash(const std::string& modo, const std::string& hash_path, long chave);

// Preenche o índice hash de index_path com os dados da tabela de mtd
void preenche_indice_hash(const Metadado& mtd, const std::string& index_path, const std::string& chave);

// Funções internas

// Retorna em qual dos baldes iniciais deve ser inserido/buscado
long hash_func(long chave, long M);

// Insere p no bloco da posição index de arquivo
void insere_bloco(FILE *arquivo, long index, Par p);

// Salva bloco blk na posição pos de arquivo, arquivo já deve estar aberto
long salva_bloco(FILE *arquivo, long pos, Bloco blk);

// Retorna o bloco em arquivo a partir de pos, arquivo já deve estar aberto
Bloco le_bloco(FILE* arquivo, long pos);

bool operator>(Par a, Par b);
bool operator<(Par a, Par b);

// Função unicamente para DEBUG
void imprime_hash(const std::string& path);

#endif
