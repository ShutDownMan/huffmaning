
clear && gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c src/trie.c src/dynamic_array.c -o huffman && gdb -ex "run" -ex "bt" --args ./huffman -C -t 0 1mb.txt test_int.huffed
clear && gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c src/trie.c src/dynamic_array.c -o huffman && gdb -ex "run" -ex "bt" --args ./huffman -D test_int.huffed test_out.txt
