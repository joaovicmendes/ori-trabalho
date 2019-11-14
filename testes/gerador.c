#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    char *stringue;
    stringue = malloc(sizeof(char) * 10);

    if (argc < 2)
        printf("Argumentos insuficientes\n");
    else
    {
        FILE *arquivo = fopen("teste.txt", "w+");
        if (arquivo == NULL)
        {
            printf("Não conseguiu abrir arquivo\n");
            exit(1);
        }
        fprintf(arquivo, "CT ALUNOS STR:NOME;INT:ID\n");
        for (int i = 0; i < atoi(argv[1]); i++)
        {
            fprintf(arquivo, "IR ALUNOS %s;%d\n", rand_string(stringue, 10) ,(rand() % 100));
        }
        // fprintf(arquivo, "CI H ALUNOS ID\n");
        // fprintf(arquivo, "BR N ALUNOS ID:%d\n", (rand() % 100));
        fprintf(arquivo, "EB");
        fclose(arquivo);
    }
    return 0;
}