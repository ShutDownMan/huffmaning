clear &&
gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c -o huffman &&
gdb -batch -ex "run" -ex "bt" --args ./huffman -C -t 1 test_in.txt test_int.huffed
# gdb -batch -ex "run" -ex "bt" --args ./huffman -D -t 1 test_int.huffed test_out.txt
