CC = gcc
CC_FLAGS = -g -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion -fPIC
SHARED_FLAGS = -shared


EXEC=test
SHARED=libset.so
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@


test: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

lib: $(OBJECTS)
	$(CC) $(SHARED_FLAGS) $(OBJECTS) -o $(SHARED)

clean:
	-rm -f $(EXEC) $(OBJECTS) $(SHARED)

check:
	valgrind --leak-check=yes --track-origins=yes ./$(EXEC)

gdb:
	gdb $(EXEC)

