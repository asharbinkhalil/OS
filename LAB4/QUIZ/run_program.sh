#! /usr/bin/bash
gcc -o binaries/capitalize  capitalizestr.c
gcc -o binaries/length lengthstr.c 
gcc -o binaries/reverse  reversestr.c
gcc -o binaries/main main.c 
./binaries/main