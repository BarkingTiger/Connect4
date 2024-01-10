CC = g++
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: connect4

connect4: solver.o
	$(CC) -o $@ $^

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f connect4 *.o

format:
	clang-format -i -style=file *.[ch]
