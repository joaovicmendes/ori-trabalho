#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <functional>

#include "./headers/comandos.h"
#include "./headers/hash.h"
#include "./headers/metadados.h"
#include "./headers/registros.h"

void inicializa_hash(const std::string& hash_path, long num_elem)
{
    long qtd_blocos;
    long pos;
    double aux;
    FILE *arquivo;
    Par p;
    Bloco blk;

    // Obtem número minimo de blocos para encaixar conteúdo
    aux = (double)num_elem / (double)BLOCK_CAP;
    // Considera que essa quatidade deve ser 70% dos blocos totais
    qtd_blocos = (long)ceil(aux / 0.7);

    // Criando par vazio
    p.chave = -1;
    p.cont  = -1;

    // Criando bloco vazio
    blk.num_elem = 0;
    blk.prox = -1;
    for (long i = 0; i < BLOCK_CAP; i++)
        blk.arr[i] = p;

    arquivo = fopen(hash_path.c_str(), "wb+");
    if (arquivo != NULL)
    {
        // Escrevendo número de buckets inicialmente
        fwrite(&qtd_blocos, sizeof(qtd_blocos), 1, arquivo);
        pos = ftell(arquivo);

        // Escrevendo blocos vazios
        for (long i = 0; i < qtd_blocos; i++)
            pos = salva_bloco(arquivo, pos, blk);    

        fclose(arquivo);
    }
    else
    {
        std::cout << "Erro ao criar o arquivo ./hash/" + hash_path + "\n";
        exit(1);
    }
}

void insere_hash(const std::string& hash_path, Par p)
{
    long qtd_blocos;
    long index;

    FILE *arquivo = fopen(hash_path.c_str(), "rb+");
    if (arquivo == NULL)
    {
        std::cout << "Erro ao abrir o arquivo " + hash_path + "\n";
        exit(1);
    }

    // Obtendo número de blocos (iniciais) na hash em questão
    fread(&qtd_blocos, sizeof(qtd_blocos), 1, arquivo);

    // Indice é qual bloco deve ser inserido, obtendo o número do bloco - 1 e 
    // multiplicando por BLOCK_SIZE
    index = (hash_func(p.chave, qtd_blocos) * BLOCK_SIZE) + sizeof(qtd_blocos);

    insere_bloco(arquivo, index, p);
    
    fclose(arquivo);
}

std::vector<long> busca_hash(const std::string& modo, const std::string& hash_path, long chave)
{
    bool atingiu_max = 0;
    long qtd_blocos;
    long index;
    std::vector<long> resultados;
    Bloco blk;

    FILE *arquivo = fopen(hash_path.c_str(), "rb+");
    if (arquivo == NULL)
    {
        std::cout << "Erro ao abrir o arquivo " + hash_path + "\n";
        exit(1);
    }

    // Obtendo número de blocos (iniciais) na hash em questão
    fread(&qtd_blocos, sizeof(qtd_blocos), 1, arquivo);

    // Indice é qual bloco deve ser lido, obtendo o número do bloco - 1 e 
    // multiplicando por BLOCK_SIZE
    index = (hash_func(chave, qtd_blocos) * BLOCK_SIZE) + sizeof(qtd_blocos);

    do
    {
        blk = le_bloco(arquivo, index);
        index = blk.prox;
        for (long i = 0; i < blk.num_elem; i++)
        {
            if (blk.arr[i].chave == chave)
            {
                resultados.push_back(blk.arr[i].cont);
                if (modo == "U")
                    atingiu_max = true;
            }
        }
    } while (!atingiu_max && blk.prox != -1);
    
    fclose(arquivo);

    return resultados;
}

long hash_func(long chave, long m)
{
    long a = 9491, b = 11923;
    return ((a * chave) + b) % m;
}

void insere_bloco(FILE *arquivo, long index, Par p)
{
    long pos;
    Par aux;
    Bloco blk;

    // Lê bloco
    blk = le_bloco(arquivo, index);

    // Se o bloco não está cheio
    if (blk.num_elem < BLOCK_CAP)
    {
        // Insere elemento
        blk.arr[blk.num_elem] = p;
        blk.num_elem++;

        // Ordena as posições válidas
        // std::sort(std::begin(blk.arr), std::begin(blk.arr) + blk.num_elem, std::less<Par>());       

        salva_bloco(arquivo, index, blk);
    }
    // Se está cheio, e existe um próximo
    else if (blk.prox != -1)
    {
        insere_bloco(arquivo, blk.prox, p);
    }
    else
    {
        fseek(arquivo, 0, SEEK_END);
        pos = ftell(arquivo);

        // Atualiza bloco indicando que existe uma extenção
        blk.prox = pos;
        salva_bloco(arquivo, index, blk);

        // Criando par vazio
        aux.chave = -1;
        aux.cont  = -1;

        // Criando bloco vazio
        blk.num_elem = 1;
        blk.prox = -1;
        blk.arr[0] = p;
        for (long i = 1; i < BLOCK_CAP; i++)
            blk.arr[i] = aux;

        fseek(arquivo, 0, SEEK_END);
        pos = ftell(arquivo);
        salva_bloco(arquivo, pos, blk);
    }
}

long salva_bloco(FILE *arquivo, long pos, Bloco blk)
{
    long pos_original = ftell(arquivo);
    fseek(arquivo, pos, SEEK_SET);

    fwrite(&(blk.num_elem), sizeof(blk.num_elem), 1, arquivo);
    fwrite(&(blk.prox    ), sizeof(blk.prox    ), 1, arquivo);
    for (long j = 0; j < BLOCK_CAP; j++)
    {
        fwrite(&(blk.arr[j].chave), sizeof(blk.arr[j].chave), 1, arquivo);
        fwrite(&(blk.arr[j].cont ), sizeof(blk.arr[j].cont ), 1, arquivo);
    }

    fseek(arquivo, pos_original, SEEK_SET);
    return (pos + BLOCK_SIZE);
}

Bloco le_bloco(FILE* arquivo, long pos)
{
    Bloco blk;
    fseek(arquivo, pos, SEEK_SET);

    fread(&(blk.num_elem), sizeof(blk.num_elem), 1, arquivo);
    fread(&(blk.prox    ), sizeof(blk.prox    ), 1, arquivo);
    for (long j = 0; j < BLOCK_CAP; j++)
    {
        fread(&(blk.arr[j].chave), sizeof(blk.arr[j].chave), 1, arquivo);
        fread(&(blk.arr[j].cont ), sizeof(blk.arr[j].cont ), 1, arquivo);
    }

    return blk;
}

// Sobrecarga para ordenação
bool operator>(Par a, Par b) { return (a.chave > b.chave); }
bool operator<(Par a, Par b) { return (a.chave < b.chave); }

void imprime_hash(const std::string& path)
{
    printf("Imprimindo\n");
    FILE* arquivo = fopen(path.c_str(), "rb+");

    Bloco blk;
    long qtd_blocos;

    rewind(arquivo);
    fread(&(qtd_blocos), sizeof(qtd_blocos), 1, arquivo);

    while (!feof(arquivo))
    {
        long pos = ftell(arquivo);
        blk = le_bloco(arquivo, pos);
        
        if (!feof(arquivo))
        {
            printf(" Indice ini: %ld\n", pos);

            printf(" Num. elem: %ld\n", blk.num_elem);
            printf(" Prox. blk: %ld\n", blk.prox);
            for (long j = 0; j < BLOCK_CAP; j++)
                printf(" arr[%ld] = (%ld, %ld)\n", j, blk.arr[j].chave, blk.arr[j].cont);
            printf("\n");
        }
    }
}

void preenche_indice_hash(const Metadado& mtd, const std::string& index_path, const std::string& chave)
{
    long         qtd_blocos;
    long              index;
    FILE*      arquivo_hash;
    std::fstream    arquivo;
    std::string path_tabela("./tabelas/" + mtd.get_tabela() + ".dat");
    std::string       linha;
    std::vector<Reg> campos;
    Par                   p;

    // Construindo hash
    arquivo.open(path_tabela, std::fstream::in);
    if (!arquivo.is_open())
    {
        std::cout << "Não pode abrir arquivo de tabela '" << path_tabela << "'\n";
        EB();
    }

    arquivo_hash = fopen(index_path.c_str(), "rb+");
    if (arquivo_hash == NULL)
    {
        std::cout << "Erro ao abrir o arquivo " + index_path + "\n";
        exit(1);
    }

    // Obtendo número de blocos (iniciais) na hash em questão
    fread(&qtd_blocos, sizeof(qtd_blocos), 1, arquivo_hash);

    p.cont = arquivo.tellg();
    getline(arquivo, linha);

    while (!arquivo.eof()) 
    {
        // Verificando se o campo foi deletado
        if (linha.find(":") == std::string::npos)
        {
            linha = linha.substr(0, linha.find("#"));
            Registro reg(mtd, linha);
            campos = reg.lista_campos();

            for (int i = 0; i < campos.size(); i++)
            {
                if (campos.at(i).nome_campo == chave)
                {
                    p.chave = stol(campos.at(i).valor);

                    // Indice é qual bloco deve ser inserido, obtendo o número
                    // do bloco - 1 e multiplicando por BLOCK_SIZE
                    index = (hash_func(p.chave, qtd_blocos) * BLOCK_SIZE) + sizeof(qtd_blocos);

                    insere_bloco(arquivo_hash, index, p);
                }
            }
        }
        
        p.cont = arquivo.tellg();
        getline(arquivo, linha);
    }

    fclose(arquivo_hash);
    arquivo.close();
}
