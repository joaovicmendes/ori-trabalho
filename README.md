# Organização e Recuperação da Informação (1001350)
Trabalho da disciplina Organização e Recuperação da Informação (1001350) - UFSCar 2019/2

# Resumo
Diretórios
- `headers/` arquivos de cabeçalho (`*.h`) com documentação das funções.
- `indices/` arquivos de índices das tabelas.
- `metadados/` arquivos dos metadados do sistema (`metadados/sgdb`) e das tabelas (`<nome_tabela>_<nome_campo>.dat`).
- `tabelas/` arquivos das tabelas com seus registros (`<nome_tabela>.dat`).
- `testes/` gerador de casos de teste (e casos).

Arquivos
- `DISKTREE.CPP` arquivo com funções referentes ao índice em  Àrvore B.
- `auxiliares.cpp` arquivo com funções auxiliares do projeto.
- `comandos.cpp` arquivo com funções referentes aos comandos suportados do mini SGDB.
- `hash.cpp` arquivo com funções referentes ao índice em Hash.
- `interpretador.cpp` arquivo com funções referentes à análise dos comandos, e chamada das funções adequadas.
- `main.cpp` arquivo com loop de execução principal e com modo de execução (arquivo ou interativo).
- `metadados.cpp` arquivo com funções referentes à manutenção e atualização de metadados.
- `reaproveitamento.cpp` arquivo com funções referentes ao reaproveitamento de espaço nas tabelas.
- `registros.cpp` arquivo com funções referentes à criação e manipulação de registros.
- `resultados.cpp` arquivo com funções referentes à busca sequêncial.
- `roteiro.txt` caso de teste curto, para testar durante o desenvolvimento das funções.
