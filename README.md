# Organização e Recuperação da Informação (1001350)
Trabalho da disciplina Organização e Recuperação da Informação (1001350) - UFSCar 2019/2

# Resumo
Diretórios
- [`headers/`](https://github.com/joaovicmendes/ori-trabalho/tree/master/headers) arquivos de cabeçalho (`*.h`) com documentação das funções.
- [`indices/`](https://github.com/joaovicmendes/ori-trabalho/tree/master/indices) arquivos de índices das tabelas.
- [`metadados/`](https://github.com/joaovicmendes/ori-trabalho/tree/master/metadados) arquivos dos metadados do sistema (`metadados/sgdb`) e das tabelas (`tabela_campo.dat`).
- [`tabelas/`](https://github.com/joaovicmendes/ori-trabalho/tree/master/tabelas) arquivos das tabelas com seus registros (`tabela.dat`).
- [`testes/`](https://github.com/joaovicmendes/ori-trabalho/tree/master/testes) gerador de casos de teste (e casos).

Arquivos
- [`disktree.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/disktree.cpp) arquivo com funções referentes ao índice em  Àrvore B.
- [`auxiliares.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/auxiliares.cpp) arquivo com funções auxiliares do projeto.
- [`comandos.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/comandos.cpp) arquivo com funções referentes aos comandos suportados do mini SGDB.
- [`hash.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/hash.cpp) arquivo com funções referentes ao índice em Hash.
- [`interpretador.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/interpretador.cpp) arquivo com funções referentes à análise dos comandos, e chamada das funções adequadas.
- [`main.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/main.cpp) arquivo com loop de execução principal e com modo de execução (arquivo ou interativo).
- [`metadados.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/metadados.cpp) arquivo com funções referentes à manutenção e atualização de metadados.
- [`reaproveitamento.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/reaproveitamento.cpp) arquivo com funções referentes ao reaproveitamento de espaço nas tabelas.
- [`registros.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/registros.cpp) arquivo com funções referentes à criação e manipulação de registros.
- [`resultados.cpp`](https://github.com/joaovicmendes/ori-trabalho/blob/master/resultados.cpp) arquivo com funções referentes à busca sequêncial.
- [`roteiro.txt`](https://github.com/joaovicmendes/ori-trabalho/blob/master/roteiro.cpp) caso de teste curto, para testar durante o desenvolvimento das funções.

# Grupo
- Gabrielle Faria
- Henrique Frajacomo
- [João Victor Mendes Freire](https://github.com/joaovicmendes)
- Juan Santos
- Murilo Carmo
- [Yuri Gabriel Aragão da Silva](https://github.com/Tyred)

# Ciclo 1 – Interpretador de comandos e estruturação dos metadados para a base
  Existem dois tipos de metadados: do sistema e de cada tabela.

  Os metadados do sistema estão armazenados no arquivo `metadados/sgdb`, e contem apenas o nome das tabelas existentes na base. Estão no formato `[TAM_NOME_TABELA][NOME_TABELA]`.

  Os metadados de cada tabela estão armazenados no arquivo `metadados/nome_tabela.dat`, e contém dados no formato `[POS_REMOVIDOS][TAM_NOME_TABELA][NOME_TABELA][NUM_CAMPOS][TAM_TIPO_CAMPO][TIPO_CAMPO][TAM_NOME_CAMPO][NOME_CAMPO][TIPO_INDICE]...[INI_REMOVIDOS]`.


# Ciclo 2 – Criação e remoção de tabela e operações de inserção e busca
  Os resultados de busca de uma seção são armazenados em um `std::map` em que a chave é o nome da tabela e o conteúdo é um `std::vector<long>`, com as posições enocntradas.

  As operações de inserções são feitas separando cada campo por `;` e diferentes registros por `\n`. Campos binários tem os valores reservados substituidos por `<t0k3n0xXX>`, em que `XX` se refere ao código do carácter internamente.

  As operações de remoção de registro e de apresentação de resultados abrem o arquivo e posicionam o ponteiro nas posições presentes no vector.


# Ciclo 3 - Remoção de registros e implementação da estratégia de reuso de espaço
  Para marcar uma remoção, percisamos escrever no arquivo `prox:tam#`, onde prox e tam são `long`. Por isso, acabou sendo necessário definir um tamanho mínimo de registro, de forma que a marcação de remoção não sobrescrevesse partes indevidas do arquivo. Definiu-se o tamanho de 40 bytes como mínimo, e registros menores que isso tem uma fragmentação interna, marcada por vários `#`.

  O reaproveitamento de espaço utiliza a estratégia first-fit. Um ponteiro para a primeira posição removida é mantido nos metadados, e o campo `prox` na demarcação indica o próximo campo removido, enquanto `tam` indica o tamanho em bytes do registro atual.

  Se, ao adicionarmos um novo registro numa posição que havia sido removida anteriormente, "sobrarem" mais de 41 bytes, então adicionamos um `\n` ao fim dos dados inseridos (levando em conta o valor mínimo) e os demais bytes são tratados como uma nova posição à ser reutilizada (é adicionada à lista de removidos com os campos prox e tam atualizados). Se sobram menos que 41 bytes, fragmentação interna é adicionada.


# Ciclo 4 – Incorporação da indexação aos metadados
  Valor [TIPO_INDICE] foram adicionados aos metadados da tabela para cada campo, nos quais `N` que dizer que não há índice, `A` quer dizer índice em àrvore B e `H` índice em hash.


# Ciclo 5 – Indexação com árvores de multiplos caminhos
  Utilizada implementação de um livro com alterações (implementação e créditos `em disktree.cpp`).


# Ciclo 6 – Indexação com hash
  Seja a, b números primos, k a chave e M o número de baldes iniciais, a função de hashing é (a * k + b) mod M. Esse valor indica em qual dos baldes iniciais o conteúdo deveria ser adicionado, e deve ser multiplicado por BLOCK_SIZE para encontrar o endereço do bloco no arquivo. É uma função clássica, de custo computacional baixo, determinista com distribuição uniforme.

  Cada balde tem um vetor de chaves, de tamanho predefinido. Uma chave é adicionada na sua posição, de forma a manter o vetor ordenado. Se o vetor estiver cheio, um novo bloco é adicionado ao fim, como extensão do atual, e seu endereço inicial é registrado no bloco que o deu origem. A chave é então adicionada nele.

  No momento de criação do índice, a tabela é percorrida, contando o número de registros. Esse número então é considerado 60% da capacidade, e o número correspondente à 100% é alocado.
