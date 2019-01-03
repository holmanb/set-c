MAIN=obj
EXEC=a.out
$(MAIN).o: $(MAIN).c
	gcc -g $(MAIN).c -o $(EXEC)

test:
	./$(EXEC)

clean:
	rm $(EXEC)

check:
	valgrind --leak-check=yes ./a.out
