CC = gcc
CC_FLAGS = -g -Wall

EXEC=test
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)



clean:
	-rm -f $(EXEC) $(OBJECTS)

check:
	valgrind --leak-check=yes --track-origins=yes ./$(EXEC)

gdb:
	gdb $(EXEC)

