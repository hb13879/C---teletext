CFLAGS = `sdl2-config --cflags` -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS = SDL_Wrap.h teletext.h
TARGET = teletest crash01 crash02 teletext_wrongsize
SOURCES =  teletext.c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
TELETEXTFILE = "panda.m7"
CRASHFILE1 = "over_1000_bytes.m7"
CRASHFILE2 = "under_1000_bytes.m7"
BADOPCODE = "bad_opcode.m7"

all: $(TARGET)

teletest: teletext_testing.c $(SOURCES) $(INCS)
	$(CC) teletext_testing.c $(SOURCES) -o teletest $(CFLAGS) $(LIBS)

crash01: crash_test01.c $(SOURCES) $(INCS)
	$(CC) crash_test01.c $(SOURCES) -o crash01 $(CFLAGS) $(LIBS)

crash02: crash_test02.c $(SOURCES) $(INCS)
	$(CC) crash_test02.c $(SOURCES) -o crash02 $(CFLAGS) $(LIBS)

teletext_wrongsize: $(SOURCES) $(INCS)
	$(CC) RUN_teletext.c $(SOURCES) -o teletext_wrongsize $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	-./crash01
	-./crash02 $(TELETEXTFILE)
	-./teletext_wrongsize $(CRASHFILE1)
	-./teletext_wrongsize $(CRASHFILE2)
	-./teletext_wrongsize $(BADOPCODE)
	./teletest $(TELETEXTFILE)
