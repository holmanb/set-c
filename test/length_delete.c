#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "set.h"

#define PASS "PASS - "
#define FAIL "FAIL - "
#define die(msg) printf("Error thrown %s:%d:\n"msg ,__FILE__, __LINE__);raise(SIGSEGV)
#define assert(cond, m) \
    if(!(cond)){die(m);}

/*simplify and standardize response malloc*/
void * xalloc(size_t size){
    void * ptr = malloc(size);
    if(ptr == NULL){
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void * xrealloc(void * p, size_t size){
    void * ptr = realloc(p, size);
    if(ptr == NULL){
        printf("Error reallocating memory\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int main(void){

	int i0;
	int *i1 = xalloc(sizeof(int));
	int *i2 = xalloc(sizeof(int));
	int *i3 = xalloc(sizeof(int));
	int *i4 = xalloc(sizeof(int));
	int *i5 = xalloc(sizeof(int));
	char *c4 = xalloc(sizeof(char));
	struct set *new_set2;

	i0 = 512;
	*i1 = 1024;
	*i2 = 2048;
	*i3 = 4096;
	*i4 = 8192;
	*i5 = 8192;
	*c4 = 'c';

	new_set2 = set_init();

	set_add(new_set2, &i0, INT);
	set_add(new_set2, i1, INT);
	set_add(new_set2, i2, INT);
	set_add(new_set2, i3, INT);
	set_add(new_set2, c4, CHAR);

	// i4 is equal to i5, so this should fail
	set_add(new_set2, i4, INT);
	set_add(new_set2, i5, INT);

	unsigned int n=set_length(new_set2);

	set_delete(new_set2, i4, INT);
	assert(n-1==(set_length(new_set2)), FAIL "set_delete() or set_length() is broken\n");

	set_delete(new_set2, i2, INT);
	assert(n-2==(set_length(new_set2)), FAIL "set_delete() or set_length() is broken\n");

	set_delete(new_set2, i1, INT);
	assert(n-3==(set_length(new_set2)), FAIL "set_delete() or set_length() is broken\n");

	set_delete(new_set2, i3, INT);
	assert(n-4==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

	set_delete(new_set2, c4, CHAR);
	assert(n-5==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

	printf(PASS "set_delete() and set_length() tests \n");

	set_free(new_set2);
	free(i1);
	free(i2);
	free(i3);
	free(c4);
	free(i4);
	free(i5);

	return 0;
}

