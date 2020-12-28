CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -z execstack

.PHONY: all
all: chall

chall: chall.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) *.o chall
