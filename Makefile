CC = clang -g
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: connect4

connect4: connect4.o ai.o TranspositionTable.o
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f connect4 *.o

format:
	clang-format -i -style=file *.[ch]
