MAIN=obj
EXEC=a.out
$(MAIN).o: $(MAIN).c
	gcc -g $(MAIN).c -o $(EXEC) -Wall -Werror -Wcast-align

test:
	./$(EXEC)

clean:
	rm $(EXEC)

check:
	valgrind --leak-check=yes --track-origins=yes ./a.out

gdb:
	gdb $(EXEC)
