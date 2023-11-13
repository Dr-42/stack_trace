set -e
clang -g -Wall -Wextra test.c -o main -ldl -rdynamic
./main
rm main
