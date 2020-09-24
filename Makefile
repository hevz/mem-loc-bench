CC=gcc
CFLAGS=-O2

all : bench
	./bench 0x66737b8
	./bench 0xff7d595888

bench : bench.c bench.S
	$(CC) $(CFLAGS) -o $@ $^
