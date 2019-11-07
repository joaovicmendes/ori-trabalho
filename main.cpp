#include <iostream>
#include <cstdio>

#include "./headers/auxiliares.h"
#include "./headers/comandos.h"
#include "./headers/interpretador.h"

#define ARQUIVO    1
#define INTERATIVO 0
#define CMD_MAX    200

int main (int argc, char **argv)
{
    int modo_exec;
    FILE *arquivo = NULL;
    
    // Definindo o modo de execução
    if (argc == 2) 
    {
        modo_exec = ARQUIVO;
        arquivo   = fopen_safe(argv[1], "r");
    }
    else 
    {
        modo_exec = INTERATIVO;
        arquivo   = stdin;
    }

    // Se conseguiu abrir o arquivo com sucesso, entra no loop de leitura
    // Esta verificação fica a cargo do fopen_safe()
    char   *buffer = NULL;
    size_t buff_sz = 0;

    if (modo_exec == INTERATIVO)
        printf("> ");

    while (getline(&buffer, &buff_sz, arquivo) != -1) 
    {
        interpreta(buffer);

        if (modo_exec == INTERATIVO)
            printf("> ");
    }
    if (buffer != NULL)
        free(buffer);
    if (modo_exec == ARQUIVO)
        fclose(arquivo);

    return 0;
}
