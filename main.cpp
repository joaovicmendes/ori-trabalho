#include <iostream>
#include <cstdio>
#include "./headers/interpretador.h"
#include "./headers/comandos.h"

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
        arquivo = fopen(argv[1], "r");
    }
    else 
    {
        modo_exec = INTERATIVO;
        arquivo = stdin;
    }

    // Se conseguiu abrir o arquivo com sucesso, entra no loop de leitura
    if (arquivo != NULL) 
    {
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
    } 
    else 
    {
        printf("Problemas ao abrir o arquivo '%s'\n", argv[1]);
        EB();
    }

    return 0;
}
