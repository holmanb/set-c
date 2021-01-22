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


struct usr_defined_custom{
    int i;
    char j;
};

int string_equality_function(void *ut1, void * ut2);
int string_equality_function(void *ut1, void * ut2){
    return !strcmp((char *)ut1,(char*)ut2);
}
int string_print(void *val);
int string_print(void *val){
    printf("%s\n", (char*)val);
    return 0;
}

// SAMPLE USER DEFINED FUNCTION:
// checks to see if the integers are equal
int test_equality_function(void * ut1, void * ut2);
int test_equality_function(void * ut1, void * ut2){
    return !(((struct usr_defined_custom *)ut1)->i != ((struct usr_defined_custom *)ut2)->i || \
    ((struct usr_defined_custom *)ut1)->j != ((struct usr_defined_custom *)ut2)->j);
}

int main(void){

	char *str4 = "test string%d";
	struct adt_funcs f;
	f.ptr_equality =  string_equality_function;
	f.ptr_print = string_print;
	//const long unsigned int size=20000;
	const long unsigned int size=8;
	struct set * s9 = set_init();
	set_add_adt(s9, &f, STRING);
	char ** bufs = xalloc(size * sizeof(char *));
	for (long unsigned int i=0; i<size; i++){
	    char *buf = xalloc(512);
	    bufs[i] = buf;
	    snprintf(buf, 512, str4, i);
	    set_add(s9, buf, STRING);
	}
	printf("set length %d\n", set_length(s9));
	set_print(s9);
	set_free(s9);
	for (long unsigned int i=0; i<size; i++){
	    free(bufs[i]);
	}
	free(bufs);

	return 0;
}

