#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"
#include "./headers/metadados.h"
#include "./headers/registros.h"
#include "./headers/resultados.h"

std::map<std::string, std::vector<long int> > lista_resultados;

void EB() 
{
    std::cout << "Encerrando execução\n";
    exit(0);
}

void LT() 
{
    char *buffer = NULL;
    int buff_sz = 0;

    FILE *arquivo = fopen_safe(SGBD_PATH, "ab+");
    rewind(arquivo);

    if (fread(&buff_sz, sizeof(buff_sz), 1, arquivo))
    {
        std::cout << "Listando tabelas: \n";
        do {

        // Alocando buffer e lendo sequência
        buffer = (char *) malloc_safe(buff_sz + 2);
        fread(buffer, sizeof(char), buff_sz, arquivo);
        buffer[buff_sz] = '\0';

        std::cout << " '" << buffer << "'\n";

        free(buffer);
        } while (fread(&buff_sz, sizeof(buff_sz), 1, arquivo));
    }
    else
        std::cout << "Não há tabelas na base de dados\n";

    fclose(arquivo);
}

void CT(const std::string& tabela, const std::string& campos) 
{
    Campo      c;
    Metadado mtd;
    FILE *arquivo;

    if (tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' já existe na base de dados\n";
        EB();
    }

    mtd.set_tabela(tabela);

    // Inserindo tabela nos metadados do sistema
    insere_tabela(SGBD_PATH, tabela);

    // Recortando campos e salvando os metadados
    std::vector<std::string> vec = str_tokenize(campos, ';');
    for (int i = 0; i < vec.size(); i++)
    {
        std::vector<std::string> subvec = str_tokenize(vec.at(i), ':');
        c.tipo = subvec.at(0);
        c.nome = subvec.at(1);
        c.indice = 'N';
        mtd.add_campo(c);
    }
    mtd.save();

    // Criando arquivo da tabela (vazio)
    std::string path("./tabelas/" + tabela + ".dat");
    arquivo = fopen_safe(path.c_str(), "wb");
    fclose(arquivo);

    std::cout << "Tabela '" + tabela + "' adicionada com sucesso\n";
}

void BR(const std::string& modo, const std::string& tabela, const std::string& busca) 
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    Metadado mtd(tabela);
    std::vector<long int> resultados;

    // Recortando busca em campo:chave
    std::vector<std::string> vec = str_tokenize(busca, ':');
    if (vec.size() != 2)
        erro_sintaxe(2);

    // Procurando se existe o campo e qual seu indice
    char indice_campo = mtd.indice_em( vec.front() );
    if  (indice_campo == '\0')
        erro_sintaxe(2);

    // Imprimindo modo de operação apropriado
    if (modo == "N")
        std::cout << "Buscando registros '" << vec.back() << "' em '" << vec.front() << "' na tabela '" << tabela << "'\n";
    else
        std::cout << "Buscando primeiro registro '" << vec.back() << "' em '" << vec.front() << "' na tabela '" << tabela << "'\n";


    if (indice_campo == 'H')
    {
        std::cout << "Busca em índice hash realizada (não implementada)\n";
        resultados = busca_sequencial(tabela, modo, vec.front(), vec.back());
    }
    else if (indice_campo == 'A')
    {
        std::cout << "Busca em índice árvore realizada (não implementada)\n";
        resultados = busca_sequencial(tabela, modo, vec.front(), vec.back());
    }
    else
    {
        std::cout << "Busca sequencial realizada\n";
        resultados = busca_sequencial(tabela, modo, vec.front(), vec.back());
    }

    std::map<std::string, std::vector<long int> >::iterator it;
    it = lista_resultados.find(tabela);
    if (it != lista_resultados.end())
    {
        it->second = resultados;
    }
    else
    {
        lista_resultados[tabela] = resultados;
    }
}

void CI(const std::string& modo, const std::string& tabela, const std::string& chave) 
{
    std::cout << "Comando 'CI' '" + modo + "' '" + tabela + "' '" + chave + "' (não implementado)\n";
}

void IR(const std::string& tabela, const std::string& registro) 
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    Metadado mtd(tabela);
    Registro reg(mtd, registro);
    std::cout << "Inserindo registro na tabela '" << tabela << "'\n";
    reg.print();
    reg.append();
}

void RI(const std::string& tabela, const std::string& chave) 
{
    std::cout << "Comando 'RI' '" + tabela + "' '" + chave + "' (não implementado)\n";    
}

void GI(const std::string& tabela, const std::string& chave) 
{
    std::cout << "Comando 'GI' '" + tabela + "' '" + chave + "' (não implementado)\n";    
}

void RT(const std::string& tabela) 
{
    std::string path;
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela não existente na base de dados\n";
        EB();
    }

    // Removendo dos metadados do sistema
    remove_tabela(tabela);

    // Rmovendo o arquivo dos metadados da tabela
    path.assign("./metadados/" + tabela + ".dat");
    remove(path.c_str());

    // Rmovendo o arquivo da tabela
    path.assign("./tabelas/" + tabela + ".dat");
    remove(path.c_str());

    std::cout << "Tabela '" << tabela << "' removida com sucesso\n";
}

void AT(const std::string& tabela) 
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela não existente na base de dados\n";
        EB();
    }
    std::cout << "Apresentando dados de '" << tabela << "'\n";

    Metadado mtd(tabela);
    mtd.print();
}

void AR(const std::string& tabela) 
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    std::map<std::string, std::vector<long int> >::iterator it = lista_resultados.find(tabela);
    if (it == lista_resultados.end())
    {
        std::cout << "Não foram realizadas buscas na tabela '" << tabela << "'\n";
        return;
    }
    
    std::ifstream arquivo("./tabelas/" + tabela + ".dat");
    if (!arquivo.is_open())
    {
        std::cout << "Arquivo '" << "./tabelas/" + tabela + ".dat" << "' não encontrado\n";
        EB();
    }

    Metadado mtd(tabela);
    std::string linha;

    std::cout << "Apresentando resultados da última busca em '" << tabela << "'\n";
    if (it->second.size() == 0)
    {
        std::cout << "Nenhum registro encontrado\n";
        return;
    } 

    for (int i = 0; i < it->second.size(); i++)
    {
        arquivo.seekg(it->second.at(i));
        getline(arquivo, linha);

        Registro resultado(mtd, linha);
        resultado.print();

        std::cout << "  ---\n";
    }
}

void RR(const std::string& tabela) 
{
    std::cout << "Comando 'RR' '" + tabela + "' (não implementado)\n";    
}
