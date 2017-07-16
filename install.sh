#!/bin/bash

set -eu
SRCS='main.c'
PROGRAM='sci'
PREFIX=${PREFIX:-"/usr/bin/"}
gcc -s -O2 -march=native $SRCS -o $PROGRAM
install $PROGRAM $PREFIX
