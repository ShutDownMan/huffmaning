
# Trabalho de PAA - Compressão com Huffman

## Alunos

Jedson Gabriel Ferreira de Paula

## Comparativo de compressão

| Tipo | Arquivo | Tamanho original (bytes) | Tamanho comprimido (bytes) | Tempo de compressão (s) | Tempo de descompressão (s) |
|---|---|---|---|---|---|
|   word   |   2mb.txt   |   2120  | 416 | 0,232  |   0,880    |
|   word   |   20mb.txt   |   21172  | 4080 | 1,247   |   0,790    |
|   word   |   100mb.txt   |   105848  | 20368 | 11,427   |   4,020    |
|   char   |    2mb.txt  |   2120  |  416 | 0,127   |   0,182    |
|   char   |    20mb.txt  |   21172  |  4080 | 1,380   |   1,706    |
|   char   |    100mb.txt  |   105848  |  20368 | 6,903   |   8,874    |

## Metodologia

Foram executados uma bateria de 10 execuções para cada arquivo, e os valores apresentados são a média dos resultados obtidos.

Para conseguir o tempo de execução, foi utilizado o programa time do linux, que retorna o tempo de execução de um programa e retirado o tempo real.

