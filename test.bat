
@REM clear && gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c src/trie.c src/dynamic_array.c -o huffman && gdb -ex "run" -ex "bt" --args ./huffman -C -t 0 1mb.txt test_int.huffed
@REM clear && gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c src/trie.c src/dynamic_array.c -o huffman && gdb -ex "run" -ex "bt" --args ./huffman -D test_int.huffed test_out.txt

@REM clear && gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c src/trie.c src/dynamic_array.c -o huffman && gdb -ex "run" -ex "bt" --args ./huffman -C -t 1 10MB.txt test_int.huffed
@REM clear && gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c src/trie.c src/dynamic_array.c -o huffman && gdb -ex "run" -ex "bt" --args ./huffman -D test_int.huffed test_out.txt

clear && gcc -O2 src/main.c src/priority_queue.c src/bitvector.c src/huffman.c src/trie.c src/dynamic_array.c -o huffman && ./huffman -C -t 0 10MB.txt test_int.huffed
clear
time ./huffman -C -t 0 10MB.txt test_int.huffed
time ./huffman -C -t 0 10MB.txt test_int.huffed
time ./huffman -C -t 0 10MB.txt test_int.huffed
time ./huffman -C -t 0 10MB.txt test_int.huffed
time ./huffman -C -t 0 10MB.txt test_int.huffed

time ./huffman -D test_int.huffed test_out.txt
time ./huffman -D test_int.huffed test_out.txt
time ./huffman -D test_int.huffed test_out.txt
time ./huffman -D test_int.huffed test_out.txt
time ./huffman -D test_int.huffed test_out.txt

