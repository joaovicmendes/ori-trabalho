#include <iostream>
#include <string>
#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/metadados.h"

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

    if (tem_tabela(tabela))
    {
        std::cout << "Tabela '" << tabela << "' já existe na base de dados\n";
        EB();
    }

    mtd.set_tabela(tabela);

    // Inserindo tabela nos metadados do sistema
    insere_tabela(SGBD_PATH, tabela);

    // Recortando campos e adicionando aos metadados
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
}

void BR(const std::string& modo, const std::string& tabela, const std::string& busca) 
{
    std::cout << "Comando 'BR' '" + modo + "' '" + tabela + "' '" + busca + "' (não implementado)\n";       
}

void CI(const std::string& modo, const std::string& tabela, const std::string& chave) 
{
    std::cout << "Comando 'CI' '" + modo + "' '" + tabela + "' '" + chave + "' (não implementado)\n";
}

void IR(const std::string& tabela, const std::string& registro) 
{
    std::cout << "Comando 'IR' '" + tabela + "' '" + registro + "' (não implementado)\n";    
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
    std::cout << "Comando 'RT' '" + tabela + "' (não implementado)\n";    
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
    std::cout << "Comando 'AR' '" + tabela + "' (não implementado)\n";    
}

void RR(const std::string& tabela) 
{
    std::cout << "Comando 'RR' '" + tabela + "' (não implementado)\n";    
}
