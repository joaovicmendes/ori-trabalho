#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

// Encerra o programa quando um erro de sintaxe acontece. Recebe um inteiro que
// representa que tipo de erro ocorreu.
// Códigos de erro:
// 1: no comando
// 2: nos argumentos (quantidade ou formatação)
// 3: nos tipos dos argumentos
// 4: espaços indevidos
// @param int: código do erro
void erro_sintaxe(int tipo);

// Recebe uma cadeia de caracteres de um comando e o decodifica, enviando para 
// a função adequada. Pode chamar erro de sintaxe caso não reconheça o comando.
// @param char *: string a ser decodificada
void interpreta(char *comando);

#endif
