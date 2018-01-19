CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
CFLAGS* = `sdl2-config --cflags`
INCS = SDL_Wrap.h teletext.h
TARGET = teletext
SOURCES = RUN_teletext.c teletext.c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
TELETEXTFILE = "custom.m7"

all: $(TARGET)

rdp:
	$(CC) rdp.c -o rdp $(CFLAGS)

teletext: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o teletext $(CFLAGS*) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./rdp
	./teletext $(TELETEXTFILE)
