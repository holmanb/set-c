#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "set.h"

#define SIZE 20000
#define PASS "PASS - "
#define FAIL "FAIL - "
#define die(msg) printf("Error thrown %s:%d:\n"msg ,__FILE__, __LINE__);raise(SIGSEGV)
#define assert(cond, m) \
    if(!(cond)){die(m);}

/*
basic testing of the functionality
this isn't a test framework, just some basic sanity checking to aid in development
in addition, this should serve as a reference for how to use the data structure
*/
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

	int i;
	struct set * s9 = set_init();
	int ints[SIZE];
	for (i=0; i<SIZE; i++){
		ints[i] = i;
		set_add(s9, &ints[i], INT);
	}
	printf("set length %d\n", set_length(s9));
	set_print(s9);
	set_free(s9);

	return 0;
}

