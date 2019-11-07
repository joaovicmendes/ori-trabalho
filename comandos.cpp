#include <iostream>
#include <string>
#include "./headers/comandos.h"

void EB() 
{
    std::cout << "Encerrando execução\n";
    exit(0);
}

void LT() 
{
    std::cout << "Comando 'LT' (não implementado)\n";
}

void BR(std::string modo, std::string tabela, std::string busca) 
{
    std::cout << "Comando 'BR' '" + modo + "' '" + tabela + "' '" + busca + "' (não implementado)\n";       
}

void CI(std::string modo, std::string tabela, std::string chave) 
{
    std::cout << "Comando 'CI' '" + modo + "' '" + tabela + "' '" + chave + "' (não implementado)\n";
}

void CT(std::string tabela, std::string campos) 
{
    std::cout << "Comando 'CT' '" + tabela + "' '" + campos + "' (não implementado)\n";    
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
