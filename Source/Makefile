CFLAGS = `sdl2-config --cflags` -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS = SDL_Wrap.h teletext.h
TARGET = teletext teletext_d
SOURCES = RUN_teletext.c teletext.c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
TELETEXTFILE = "M7/panda.m7"

default: all run

all: $(TARGET)

teletext: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o teletext $(CFLAGS) $(LIBS)

teletext_d: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o teletext_d -g -O0 $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

memchk: teletext_d
	valgrind --error-exitcode=1 --quiet --leak-check=full ./teletext_d $(TELETEXTFILE)

run: all
	./teletext $(TELETEXTFILE)
