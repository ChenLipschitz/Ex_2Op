
CC = gcc
CFLAGS = -Wall -Wextra

all: encode decode codecA codecB copy cmp stshell

encode: encode.c libcodecA.so libcodecB.so
	$(CC) $(CFLAGS) -o $@ $< -ldl

decode: decode.c libcodecA.so libcodecB.so
	$(CC) $(CFLAGS) -o $@ $< -ldl

codecA: codecA.c
	$(CC) $(CFLAGS) -shared -fPIC -o lib$@.so $<

codecB: codecB.c
	$(CC) $(CFLAGS) -shared -fPIC -o lib$@.so $<

libcodecA.so: codecA.c
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $<

libcodecB.so: codecB.c
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $<

copy: copy.c
	$(CC) $(CFLAGS) -o $@ $<

cmp: cmp.c
	$(CC) $(CFLAGS) -o $@ $<

stshell: stshell.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f encode decode libcodecA.so libcodecB.so copy cmp stshell

.PHONY: all clean
