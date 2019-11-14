#ifndef AUXILIARES_H
#define AUXILIARES_H

// Recebe um std::string e retorna true se existe espaços (" ") e false se não.
// @param std::string
// @ret   bool
bool tem_espaco(const std::string& texto);

// Recebe uma cadeia de caracteres e retorna true se o ela for um dentre 
// "INT", "FLT", "STR", "BIN".
// @param std::string
// @ret   int
bool tipo_valido(const std::string& texto);

// Aplica a função toupper(char) para uma string, mas altera na string
// passada como parâmetro, ao invés de retornar.
// @param std::string
void to_upper(std::string& texto);

// Recebe uma cadeia de caracteres e retorna a quantidade de argumentos 
// contidos na string. Utiliza o número de ";" encontrados e compara com ":"
// encontrados para validar.
// @param std::string
// @ret   int
int qtd_campos(const std::string& campos);

// Recebe uma cadeia de caracteres e retorna a quantidade de argumentos 
// contidos na string. Se utiliza do número de ; encontrados.
// @param std::string
// @ret   int
int qtd_registros(const std::string& campos);

// Implementação da função malloc() com verificação de NULL ptr
// @param unsigned
// @ret   void *
void *malloc_safe(unsigned nbytes);

// Implementação da função fopen() com verificação de NULL ptr
// @param const char* : caminho do arquivo
// @param const char* : modo de abertura
// @ret        FILE *
FILE *fopen_safe(const char *path, const char *modo);

// Retorna um vector<string> com os tokens do texto, usando delim como base
// @param std::string : texto a ser tokenizado
// @param const char* : delimitadores
// @ret   std::vector<std::string>
std::vector<std::string> str_tokenize(const std::string& texto, const char delim);

// Retorna o número de registros de tabela (conta o número de \n)
// @param  str::string : tabela na qual os registros devem ser contados
// @ret           long : num de registros
long reg_count(const std::string& tabela);

#endif
