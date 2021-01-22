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

// adt proto
int string_equality_function(void *ut1, void * ut2);
int test_equality_function(void * ut1, void * ut2);
int string_print(void *val);

struct usr_defined_custom{
    int i;
    char j;
};

int string_equality_function(void *ut1, void * ut2){
    return !strcmp((char *)ut1,(char*)ut2);
}
int string_print(void *val){
    printf("%s\n", (char*)val);
    return 0;
}

// SAMPLE USER DEFINED FUNCTION:
// checks to see if the integers are equal
int test_equality_function(void * ut1, void * ut2){
    return !(((struct usr_defined_custom *)ut1)->i != ((struct usr_defined_custom *)ut2)->i || \
    ((struct usr_defined_custom *)ut1)->j != ((struct usr_defined_custom *)ut2)->j);
}

int main(void){

	struct set * s = NULL;
	s = set_init();

	struct usr_defined_custom *c1,*c2, *c3;
	c1 = xalloc(sizeof(struct usr_defined_custom));
	c2 = xalloc(sizeof(struct usr_defined_custom));
	c3 = xalloc(sizeof(struct usr_defined_custom));

	c1->i=0;
	c1->j='c';

	c2->i=0;
	c2->j='c';

	c3->i=0;
	c3->j='d';

	struct adt_funcs test_adt;
	test_adt.ptr_equality = test_equality_function;
	set_add_adt(s,&test_adt, USER_DEFINED);

	//set_add_adt(s,&test_equality_function, USER_DEFINED);

	assert(set_length(s) == 0, FAIL "there shouldn't be any items in the set yet\n");
	set_add(s, c1, USER_DEFINED);
	assert(set_length(s) == 1, FAIL "there should only be one item in the set \n");
	set_add(s, c2, USER_DEFINED);
	assert(set_length(s) == 1, FAIL "there should only be one item in the set \n");
	set_add(s, c3, USER_DEFINED);
	assert(set_length(s) == 2, FAIL "there should only be two items in the set \n");
	printf(PASS "abstract data type support tests passed\n");


	free(c1);
	free(c2);
	free(c3);
	set_free(s);

	return 0;
}

