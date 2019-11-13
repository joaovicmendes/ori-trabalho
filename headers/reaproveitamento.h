#ifndef REAPROVEITAMENTO_H
#define REAPROVEITAMENTO_H

#define LONG_SZ sizeof(long)
#define MIN_REG_SZ (2*LONG_SZ + 2) 

// Retorna a posição onde um registro de tamanho tam pode ser adicionado, já
// faz as correções na lista encadeada
long busca_removido(Metadado& mtd, long tam);

#endif
