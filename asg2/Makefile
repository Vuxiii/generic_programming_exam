CC = g++ -std=c++20
SAN_FLAGS = -fsanitize=address  -fsanitize=undefined  -fsanitize=leak -fconcepts-diagnostics-depth=10 -Wreorder
CFLAGS = -g -Wall -Wextra -Wno-error=unused-variable -Wno-unused-parameter -Wno-error=unused-but-set-variable -Wno-error=sign-compare $(SAN_FLAGS)
OBJS = client.o src/Tree.o

client: $(OBJS)
	$(CC) $(CFLAGS) -o a.out $^

.PHONY: clean valgrind

valgrind: $(OBJS)
	$(CC) -o a.out $^
clean:        # so it should be declared phony
	rm -f src/*.o a.out *.o
