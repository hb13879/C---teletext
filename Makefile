CFLAGS = `sdl2-config --cflags` -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS = SDL_Wrap.h
TARGET = read_in read_in_d
SOURCES = read_in.c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
TELETEXTFILE = "test.m7"

all: $(TARGET)

read_in: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o read_in $(CFLAGS) $(LIBS)

read_in_d: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o read_in_d -g -O0 $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

memchk: read_in_d
	valgrind --error-exitcode=1 --quiet --leak-check=full ./read_in_d

run: all
	./read_in $(TELETEXTFILE)
