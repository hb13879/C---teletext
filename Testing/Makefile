CFLAGS = `sdl2-config --cflags` -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS = SDL_Wrap.h teletext.h
TARGET = teletest
SOURCES = teletext_testing.c teletext.c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
TELETEXTFILE = "panda.m7"

all: $(TARGET)

teletest: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o teletest $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./teletest $(TELETEXTFILE)
