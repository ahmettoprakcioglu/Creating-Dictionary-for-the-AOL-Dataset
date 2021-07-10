# Run this script to compile all programs

rm bin/ -r
mkdir bin
gcc -O3 task2.c -o bin/task2
gcc -O3 task3.c -o bin/task3
gcc -pthread -O3 task4.c -o bin/task4
gcc -pthread -O3 task5.c -o bin/task5
gcc -O3 task6.c -o bin/task6