CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
CFLAGS* = `sdl2-config --cflags`
INCS = SDL_Wrap.h teletext.h
TARGET = rdp rdp_d teletext
SOURCES = RUN_teletext.c teletext.c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
TELETEXTFILE = "custom.m7"

all: $(TARGET)

rdp:
	$(CC) rdp.c -o rdp $(CFLAGS)

rdp_d:
	$(CC) rdp.c -o rdp_d -g -O0 $(CFLAGS)

teletext: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o teletext $(CFLAGS*) $(CFLAGS) $(LIBS)

memchk: rdp_d
	valgrind --error-exitcode=1 --quiet --leak-check=full ./rdp_d

clean:
	rm -f $(TARGET)

run: all
	./rdp
	./teletext $(TELETEXTFILE)
