#!/bin/bash

build:
	gcc -std=c99 main.c file.c -o fsminishell
run:
	./fsminishell
clean:
	rm fsminishell
