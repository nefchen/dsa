#!/bin/bash
#
# Created on 04.12.2020 by nefchen.
#

clang++ -std=c++2a -O2 \
	-lSDL2 -lSDL2_ttf -lpthread \
	-o build/dsa \
	src/main.cpp \
	src/window.cpp \
	src/views/view.cpp \
	src/views/start_screen.cpp \
	src/views/widgets/label.cpp \
	src/views/widgets/cycle_menu.cpp

if [ $? -eq 0 ]; then
	./build/dsa
fi

