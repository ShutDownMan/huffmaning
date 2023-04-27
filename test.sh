clear &&
# gcc -g src/main.c src/priority_queue.c src/bitvector.c src/huffman.c -o huffman &&
# gdb -batch -ex "run" -ex "bt" --args ./huffman -C -t 1 100mb.txt test_int.huffed > encode.log
# gdb -batch -ex "run" -ex "bt" --args ./huffman -D -t 1 test_int.huffed test_out.txt > decode.log

gcc -o2 src/main.c src/priority_queue.c src/bitvector.c src/huffman.c -o huffman &&
time ./huffman -C -t 0 100mb.txt test_int.huffed > encode.log
time ./huffman -D -t 0 test_int.huffed test_out.txt > decode.log
