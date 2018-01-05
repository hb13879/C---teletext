CFLAGS = `sdl2-config --cflags` -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS = SDL_Wrap.h
TARGET = teletext teletext_d
SOURCES = teletext.c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
TELETEXTFILE = "lfc.m7"

all: $(TARGET)

teletext: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o teletext $(CFLAGS) $(LIBS)

teletext_d: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o teletext_d -g -O0 $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

memchk: teletext_d
	valgrind --error-exitcode=1 --quiet --leak-check=full ./teletext_d

run: all
	./teletext $(TELETEXTFILE)
