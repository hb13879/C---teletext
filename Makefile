CFLAGS = `sdl2-config --cflags` -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS = SDL_Wrap.h
TARGET = read_in
SOURCES =  $(TARGET).c SDL_Wrap.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc

all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
