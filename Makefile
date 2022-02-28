################################################################################
### This file is part of the Aerial Image Project (AIP).
###
### Copyright (c) ...
###
### The sources of AIP are distributed WITHOUT ANY WARRANTY.
################################################################################

CC = g++
CFLAGS  = -g -Wall -std=c++14

INCLUDES = -I.
LFLAGS = -lm

OBJ = Color.o Image.o Analyst.o FireSimulator.o main.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
		$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) -o $(TARGET)

main.o: main.cpp Color.h Image.h Analyst.h
		$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp -o main.o

Color.o: Color.cpp Color.h
		$(CC) $(CFLAGS) $(INCLUDES) -c Color.cpp -o Color.o

Image.o: Image.cpp Color.h Image.h
		$(CC) $(CFLAGS) $(INCLUDES) -c Image.cpp -o Image.o

Analyst.o: Analyst.cpp Color.h Image.h Analyst.h
		$(CC) $(CFLAGS) $(INCLUDES) -c Analyst.cpp -o Analyst.o

FireSimulator.o: FireSimulator.cpp Color.h Image.h Analyst.h FireSimulator.h
		$(CC) $(CFLAGS) $(INCLUDES) -c FireSimulator.cpp -o FireSimulator.o

clean:
		rm -f *~ *.o *.aip *.svg main
