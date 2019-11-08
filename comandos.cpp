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

    std::cout << "Listando tabelas: \n";
    while (!feof(arquivo))
    {
        // Lendo tamanho da sequência a seguir
        fread(&buff_sz, sizeof(buff_sz), 1, arquivo);

        // Alocando buffer e lendo sequência
        buffer = (char *) malloc_safe(buff_sz + 1);
        fread(&buffer, sizeof(char), buff_sz, arquivo);
        buffer[buff_sz] = '\0';

        if (buff_sz > 0)
            std::cout << " '" << buffer << "'\n";
        else
            std::cout << " Não há tabelas\n";

        free(buffer);
    }

    fclose(arquivo);
}

void CT(std::string tabela, std::string campos) 
{
    std::cout << "Comando 'CT' '" + tabela + "' '" + campos + "' (não implementado)\n";    
}

void BR(std::string modo, std::string tabela, std::string busca) 
{
    std::cout << "Comando 'BR' '" + modo + "' '" + tabela + "' '" + busca + "' (não implementado)\n";       
}

void CI(std::string modo, std::string tabela, std::string chave) 
{
    std::cout << "Comando 'CI' '" + modo + "' '" + tabela + "' '" + chave + "' (não implementado)\n";
}

void IR(std::string tabela, std::string registro) 
{
    std::cout << "Comando 'IR' '" + tabela + "' '" + registro + "' (não implementado)\n";    
}

void RI(std::string tabela, std::string chave) 
{
    std::cout << "Comando 'RI' '" + tabela + "' '" + chave + "' (não implementado)\n";    
}

void GI(std::string tabela, std::string chave) 
{
    std::cout << "Comando 'GI' '" + tabela + "' '" + chave + "' (não implementado)\n";    
}

void RT(std::string tabela) 
{
    std::cout << "Comando 'RT' '" + tabela + "' (não implementado)\n";    
}

void AT(std::string tabela) 
{
    std::cout << "Comando 'AT' '" + tabela + "' (não implementado)\n";    
}

void AR(std::string tabela) 
{
    std::cout << "Comando 'AR' '" + tabela + "' (não implementado)\n";    
}

void RR(std::string tabela) 
{
    std::cout << "Comando 'RR' '" + tabela + "' (não implementado)\n";    
}
