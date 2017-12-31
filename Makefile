CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS =

all: read_in read_in_d

read_in:  read_in.c $(INCS)
	$(CC) read_in.c -o read -O3 $(CFLAGS)

read_in_d:  read_in.c $(INCS)
	$(CC) read_in.c -o read_in_d -g -O $(CFLAGS)

run: all
	./read

memchk: read_in_d
	valgrind --error-exitcode=1 --quiet --leak-check=full ./read_in_d
