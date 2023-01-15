gcc main.c linked_list.c -o main.o
./main.o --num_threads=2 --num_iterations=6
mv outputs/result.txt outputs/result_1.txt
./main.o --num_threads=4 --num_iterations=8
mv outputs/result.txt outputs/result_2.txt
./main.o --num_threads=8 --num_iterations=10
mv outputs/result.txt outputs/result_3.txt
./main.o --num_threads=16 --num_iterations=12
mv outputs/result.txt outputs/result_4.txt