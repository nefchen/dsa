#!/bin/bash
#
# Created on 04.12.2020 by nefchen.
#

g++ -std=c++20 -O2 \
	-lSDL2 -lSDL2_ttf -lpthread \
	-o build/dsa \
	src/main.cpp
./build/dsa

