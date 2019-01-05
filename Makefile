MAIN=obj
EXEC=a.out
$(MAIN).o: $(MAIN).c
	gcc -g $(MAIN).c -o $(EXEC) -Wall -Werror

test:
	./$(EXEC)

clean:
	rm $(EXEC)

check:
	valgrind --leak-check=yes ./a.out

gdb:
	gdb $(EXEC)
