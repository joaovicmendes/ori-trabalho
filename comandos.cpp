#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <ctime>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"
#include "./headers/hash.h"
#include "./headers/metadados.h"
#include "./headers/registros.h"
#include "./headers/resultados.h"
#include "./headers/reaproveitamento.h"

std::map<std::string, std::vector<long> > lista_resultados;

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
    std::vector<long> resultados;

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
        std::string path("./indices/" + tabela + "_" + vec.front() + ".dat");
        std::cout << "Busca em índice hash realizada\n";
        resultados = busca_hash(modo, path, stol(vec.back()) );
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

    std::map<std::string, std::vector<long> >::iterator it;
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
    std::string index_path;
    std::fstream arquivo;

    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    Metadado mtd(tabela);
    index_path.assign("./indices/" + tabela + "_" + chave + ".dat");

    // Verificando se campo existe
    if (!mtd.tem(chave))
    {
        std::cout << "Criação de Índice inválida: campo '" << chave << "' não existente em '" << tabela << "'\n";
        EB();
    }

    // Verificando se campo é do tipo INT
    if (mtd.tipo_de(chave) != "INT")
    {
        std::cout << "Criação de Índice inválida: campo '" << chave << "' não é INT\n";
        return;
    }

    // Verificando se não existe índice
    if (mtd.indice_em(chave) != 'N')
    {
        std::cout << "Criação de Índice inválida: campo '" << chave << "' já possui índice\n";
        return;
    }

    // Criando arquivo de índice
    arquivo.open(index_path, std::fstream::out);
    if (!arquivo.is_open())
    {
        std::cout << "Não conseguiu criar arquivo de índice\n";
        EB();
    }
    arquivo.close();

    // Construindo índice hash
    if (modo == "H")
    {
        std::cout << "Criando índice hash em '" << chave << "'\n";
        inicializa_hash(index_path, reg_count(tabela));

        preenche_indice_hash(mtd, index_path, chave);

        mtd.set_indice(chave, 'H');
        mtd.save();        
    }
    // Construindo índice árvore
    else
    {
        std::cout << "Criando índice árvore em '" << chave << "' (não implementado)\n";

        mtd.set_indice(chave, 'A');
        mtd.save();
    }
}

void IR(const std::string& tabela, const std::string& registro) 
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    if (registro.find("#") != std::string::npos)
    {
        std::cout << "Inserção inválida: caracter proibido (#) encontrado\n";
        return;
    }
    else if (registro.find(":") != std::string::npos)
    {
        std::cout << "Inserção inválida: caracter proibido (:) encontrado\n";
        return;
    }

    Metadado mtd(tabela);
    Registro reg(mtd, registro);
    std::cout << "Inserindo registro na tabela '" << tabela << "'\n";
    reg.print();

    // Tratando a existência de índice hash
    std::vector<Campo> campos = mtd.get_campos();
    for (int i = 0; i < campos.size(); i++)
    {
        if (mtd.hash_em(campos.at(i).nome))
        {
            std::cout << "Inserção inválida: índice hash existente em '" << campos.at(i).nome << "'\n";
            return;
        }
    }

    // Inserindo no espaço adequado
    long pos = busca_removido(mtd, reg.length());
    if (pos != -1)
        reg.insert(pos);
    else
        reg.append();
}

void RI(const std::string& tabela, const std::string& chave) 
{
    std::string index_path("./indices/" + tabela + "_" + chave + ".dat");
    std::fstream arquivo;

    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    Metadado mtd(tabela);

    // Verificando se campo existe
    if (!mtd.tem(chave))
    {
        std::cout << "Remoção de Índice inválida: campo '" << chave << "' não existente em '" << tabela << "'\n";
        return;
    }

    // Verificando se campo é do tipo INT
    if (mtd.tipo_de(chave) != "INT")
    {
        std::cout << "Remoção de Índice inválida: campo '" << chave << "' não é INT\n";
        return;
    }

    // Verificando se ainda existe índice
    if (mtd.indice_em(chave) == 'N')
    {
        std::cout << "Remoção de Índice inválida: campo '" << chave << "' não possui índice\n";
        return;
    }

    remove(index_path.c_str());

    std::cout << "Índice em ";
    if (mtd.indice_em(chave) == 'H')
        std::cout << "hash";
    else
        std::cout << "árvore";
    std::cout << " de '" << chave << "' removido com sucesso\n";

    mtd.set_indice(chave, 'N');
    mtd.save();
}

void GI(const std::string& tabela, const std::string& chave) 
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    Metadado   mtd(tabela);
    std::string index_path;
    std::fstream   arquivo;

    // Verificando se campo existe
    if (!mtd.tem(chave))
    {
        std::cout << "Recriação de Índice inválida: campo '" << chave << "' não existente em '" << tabela << "'\n";
        return;
    }

    // Verificando se campo é do tipo INT
    if (mtd.tipo_de(chave) != "INT")
    {
        std::cout << "Recriação de Índice inválida: campo '" << chave << "' não é INT\n";
        return;
    }

    // Verificando se existe índice
    if (mtd.indice_em(chave) == 'N')
    {
        std::cout << "Recriação de Índice inválida: campo '" << chave << "' não possui índice\n";
        return;
    }

    index_path.assign("./indices/" + tabela + "_" + chave + ".dat");

    arquivo.open(index_path, std::fstream::out | std::fstream::trunc);
    arquivo.close();

    if (mtd.hash_em(chave))
    {
        inicializa_hash(index_path, reg_count(tabela));
        preenche_indice_hash(mtd, index_path, chave);
    }
    else if (mtd.arvore_em(chave))
    {

    }

    std::cout << "Índice de '" << chave << "' recriado com sucesso\n";
}

void RT(const std::string& tabela) 
{
    std::string          path;
    std::vector<Campo> campos;

    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela não existente na base de dados\n";
        EB();
    }

    Metadado mtd(tabela);

    // Removendo dos metadados do sistema
    remove_tabela(tabela);

    // Rmovendo o arquivo dos metadados da tabela
    path.assign("./metadados/" + tabela + ".dat");
    remove(path.c_str());

    // Removendo o arquivo da tabela
    path.assign("./tabelas/" + tabela + ".dat");
    remove(path.c_str());

    // Removendo índices desta tabela
    campos = mtd.get_campos();
    for (int i = 0; i < campos.size(); i++)
    {
        if (mtd.indice_em(campos.at(i).nome) != 'N')
        {
            path.assign("./indices/" + tabela + "_" + campos.at(i).nome + ".dat");
            remove(path.c_str());
        }
    }

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
    std::string linha;
    std::ifstream arquivo;
    std::map<std::string, std::vector<long> >::iterator it;

    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    it = lista_resultados.find(tabela);
    if (it == lista_resultados.end())
    {
        std::cout << "Não foram realizadas buscas na tabela '" << tabela << "'\n";
        return;
    }
    
    arquivo.open("./tabelas/" + tabela + ".dat");
    if (!arquivo.is_open())
    {
        std::cout << "Arquivo '" << "./tabelas/" + tabela + ".dat" << "' não encontrado\n";
        EB();
    }

    Metadado mtd(tabela);

    std::cout << "Apresentando resultados da última busca em '" << tabela << "'\n";

    if (it->second.size() == 0)
    {
        std::cout << "Nenhum registro encontrado\n";
    }
    else
    {
        for (int i = 0; i < it->second.size(); i++)
        {
            arquivo.seekg(it->second.at(i));
            
            getline(arquivo, linha);
            linha = linha.substr(0, linha.find("#"));
            Registro res(mtd, linha);

            res.print();
            std::cout << "  ---\n";
        }
    }
}

void RR(const std::string& tabela) 
{
    if (!tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' não existe na base de dados\n";
        EB();
    }

    char lixo;
    long indice_novo, indice_ini, tam;
    std::string path("./tabelas/" + tabela + ".dat");
    std::fstream arquivo;
    Metadado mtd(tabela);

    // Tratando a existência de índice hash
    std::vector<Campo> campos = mtd.get_campos();
    for (int i = 0; i < campos.size(); i++)
    {
        if (mtd.hash_em(campos.at(i).nome))
        {
            std::cout << "Remoção inválida: índice hash existente em '" << campos.at(i).nome << "'\n";
            return;
        }
    }

    std::map<std::string, std::vector<long> >::iterator it;
    it = lista_resultados.find(tabela);

    if (it == lista_resultados.end())
        std::cout << "Sem buscas na tabela '" << tabela << "'\n";
    else if (it->second.empty())
        std::cout << "Sem resultados da última busca na tabela '" << tabela << "'\n";
    else
    {
        arquivo.open(path);
        if (!arquivo.is_open())
        {
            std::cout << "Arquivo '" << path << "' não encontrado\n";
            EB();
        }

        for (int i = 0; i < it->second.size(); i++)
        {
            indice_ini = mtd.get_removido();

            // Vai para a posição à ser removida
            arquivo.seekg(it->second.at(i));
            indice_novo = arquivo.tellg();
            
            // Calcula o tamanho do registro atual (deletado)
            while (arquivo.peek() != '\n')
                arquivo.get(lixo);

            tam = ((long)arquivo.tellg()) - indice_novo;
            arquivo.seekg(indice_novo);

            // Escreve no registro a próxima prosição removida na forma prox:tamanho_atual#
            arquivo << std::setw(LONG_SZ) << indice_ini << ":" << std::setw(LONG_SZ) << tam << "#";

            // Atualiza o começo da lista
            mtd.set_removido(indice_novo);
        }

        arquivo.close();

        std::cout << it->second.size() <<" registro(s) removido(s) de '" << tabela << "'\n";
        lista_resultados.erase(it);
    }
}
