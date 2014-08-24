CC=gcc
CFLAGS= -g
LIBS=

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: rc4.o encrypt_conn.o test.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o test
