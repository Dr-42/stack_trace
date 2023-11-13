@echo off
sh -c "clang -g -gcodeview -O0 -Wall -Wextra test.c -o main -l dbghelp -fuse-ld=lld -Wl,--pdb="
.\main.exe
rm main.exe main.pdb
