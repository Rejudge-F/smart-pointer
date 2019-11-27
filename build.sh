#! /bin/bash

CC=g++
CFLAGS=-std=c++11 
FILENAME=`ls *.cpp`

for FILE in "$FILENAME"; do
    #statements
    "$CC" "$CFLAGS" -c "$FILE" -o "${FILE%.*}".o
done

OBJECTS=`ls *.o`
if [ -d "bin" ]; then
    #statements
    "$CC" "$CFLAGS" "$OBJECTS" -o bin/App
else
    echo "generate bin directory"
    mkdir bin 
    "$CC" "$CFLAGS" "$OBJECTS" -o bin/App
fi

rm *.o
