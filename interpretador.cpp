#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string.h>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"


void erro_sintaxe(int tipo) {
    switch (tipo) {
        case 1:
            printf("Erro no comando\n");
        break;
        case 2:
            printf("Erro nos argumentos\n");
        break;
        case 3:
            printf("Erro nos tipos\n");
        break;
        case 4:
            printf("Espaço em local não permitido\n");
        break;
    }
    EB();
}

void interpreta(char *comando) 
{
    char *arg, *tmp;
    std::string cmd, tabela, campos, registro, chave, modo, busca;

    // Separando o comando principal
    arg = strtok(comando, " \n");
    if (arg == NULL)
        return;

    cmd.assign(arg);
    to_upper(cmd);

    // Comandos que não recebem argumentos
    if (cmd == "EB") 
    {
        EB();
        return;
    }
    
    if (cmd == "LT") 
    {
        LT();
        return;
    }

    // Comandos que recebem um argumento
    // Separando o primeiro argumento
    arg = strtok(NULL, " \n");
    if (arg == NULL)
        erro_sintaxe(2);
    tabela.assign(arg);

    if (cmd == "RT") 
    {
        RT(tabela);
        return;
    }

    if (cmd == "AT") 
    {
        AT(tabela);
        return;
    }

    if (cmd == "AR") 
    {
        AR(tabela);
        return;
    }

    if (cmd == "RR") 
    {
        RR(tabela);
        return;
    }

    // Comandos que recebem dois argumentos
    arg = strtok(NULL, "\n");
    if (arg == NULL)
        erro_sintaxe(2);

    if (cmd == "CT") 
    {
        campos.assign(arg);
        CT(tabela, campos);
        return;
    }

    if (cmd == "IR") 
    {
        registro.assign(arg);
        IR(tabela, registro);
        return;
    }

    if (cmd == "RI") 
    {
        chave.assign(arg);
        RI(tabela, chave);
        return;
    }

    if (cmd == "GI") 
    {
        chave.assign(arg);
        GI(tabela, chave);
        return;
    }

    // Comandos que recebem três argumentos
    // ARG2 utilizado para separar a próxima parte do código
    tmp = strtok(arg, " ");
    if (tmp == NULL)
        erro_sintaxe(2);
    // Separando o terceiro argumento
    arg = strtok(NULL, "\0");
    if (arg == NULL)
        erro_sintaxe(2);

    if (cmd == "BR")
    {
        modo = tabela;
        to_upper(modo);

        if (modo != "N" && modo != "U")
            erro_sintaxe(3);

        tabela.assign(tmp);
        busca.assign(arg);
        BR(modo, tabela, busca);
        return;
    }

    if (cmd == "CI")
    {
        modo = tabela;
        to_upper(modo);

        if (modo != "A" && modo != "H")
            erro_sintaxe(3);

        tabela.assign(tmp);
        chave.assign(arg);
        CI(modo, tabela, chave);
        return;
    }

    erro_sintaxe(1);  
}
