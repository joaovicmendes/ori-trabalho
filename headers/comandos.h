#ifndef COMANDOS_H
#define COMANDOS_H

// Encerra a interpretação e termina a execução do programa.
void EB();

// Lista o nome de todas as tabelas existentes na base.
void LT();

// Busca em tabela os registros que satisfaçam o critério busca. Os modos de
// operação são unico ou todos.
// @param std::string : N (todos) ou U (primeiro)
// @param std::string : em qual tabela deve-se procurar
// @param std::string : o que deve ser procurado, no formado CAMPO:NOME
void BR(const std::string& modo, const std::string& tabela, const std::string& busca);

// Cria um índice estruturado em árvore ou usando hashing para certa tabela,
// usando chave como chave de busca. Atualiza os metadados.
// @param std::string : A (árvore) ou H (hashing)
// @param std::string : em qual tabela deve-se criar o índice
// @param std::string : chave de busca
void CI(const std::string& modo, const std::string& tabela, const std::string& chave);

// Cria um arquivo vazio associado ao nome tabela e o registra como ativo na 
// base, sendo campos uma lista de especificações dos campos que formarão o 
// arquivo.
// @param std::string : string que contém o nome da tabela a ser criada
// @param std::string : contém o tipo e nome dos campos da tabela no formato 
// tipo:nome, onde cada par é separado por ;
void CT(const std::string& tabela, const std::string& campos);

// Insere o registro no arquivo de tabela, usando a política de inserção
// adequada.
// @param std::string : tabela na qual deve-se realizar a operação
// @param std::string : dados do registro a ser adicionado, separados por ;
void IR(const std::string& tabela, const std::string& registro);

// Remove o índice relativo à chave, atualizando os metadados e apagando as 
// estruturas envolvidas.
// @param std::string : tabela na qual deve-se realizar a operação
// @param std::string : identificador do índice a ser apagado
void RI(const std::string& tabela, const std::string& chave);

// Gera novamente o índice de tabela referente à chave, partindo do zero.
// @param std::string : tabela na qual deve-se realizar a operação
// @param std::string : identificador do índice a ser gerado
void GI(const std::string& tabela, const std::string& chave);

// Apaga o arquivo relativo a tabela e remove seus metadados da base, bem como 
// estruturas associadas.
// @param std::string : tabela na qual deve-se realizar a operação 
void RT(const std::string& tabela);

// Apresenta um resumo dos metadados da tabela indicada: arquivos, campos e 
// índices existentes.
// @param std::string : tabela na qual deve-se realizar a operação
void AT(const std::string& tabela);

// Apresenta na tela os valores dos registros retornados pela última busca.
// @param std::string : tabela na qual deve-se realizar a operação
void AR(const std::string& tabela);

// Remove, segundo a política de remoção da tabela, todos os registros da
// última busca realizada.
// @param std::string : tabela na qual deve-se realizar a operação
void RR(const std::string& tabela);

#endif