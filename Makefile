CC = gcc
CFLAGS = -Wall -Wextra

all: copy cmp stshell

copy: copy.c
	$(CC) $(CFLAGS) $< -o $@

cmp: cmp.c
	$(CC) $(CFLAGS) $< -o $@

stshell: stshell.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f copy cmp stshell
