#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h" // isn't required for user apps, used for "xalloc()"
#include "set.h"

#define PASS "PASS - "
#define FAIL "FAIL - "

/*
basic testing of the functionality  
this isn't a test framework, just some basic sanity checking to aid in development
in addition, this should serve as a reference for how to use the data structure
*/

int main(){
        
    struct set * new_set1=NULL;
    new_set1 = set_init();
    set_free(new_set1);
    printf("set_init() and set_free() passed\n");

    struct set * new_set2=NULL;

    int *i1 = xalloc(sizeof(int)); 
    int *i2 = xalloc(sizeof(int)); 
    int *i3 = xalloc(sizeof(int)); 
    int *i4 = xalloc(sizeof(int)); 
    int *i5 = xalloc(sizeof(int)); 
    char *c4 = xalloc(sizeof(char)); 
    
    *i1 = 1024;
    *i2 = 2048;
    *i3 = 4096;
    *i4 = 8192;
    *i5 = 8192;
    *c4 = 'c';


    new_set2 = set_init();

    set_add(new_set2, i1, INT);
    set_add(new_set2, i2, INT);
    set_add(new_set2, i3, INT);
    set_add(new_set2, c4, CHAR);

    // testing set_member() function
    //printf("testing if i3 (%d) is a member: \n\t", *i3);
    assert(set_member(new_set2, i3, INT),FAIL "set_member() not working properly [1]\n");
        
    // testing set_member() function
    assert(!set_member(new_set2, i4, INT),FAIL "set_member() not working properly [2]\n");
    printf(PASS "set_member() \n");

    // testing set_member() function
    assert(set_member(new_set2, c4, CHAR),FAIL "set_member() not working properly [3]\n");
    printf(PASS "set_member() \n");

    // i4 is equal to i5, so this should fail
    set_add(new_set2, i4, INT);
    int y = set_add(new_set2, i5, INT);
    assert(y,FAIL "set_add() didn't deny adding duplicate\n");

    int n=set_length(new_set2);
    
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
    


    struct set * s = NULL;
    s = set_init();
    struct usr_defined_custom{
       int i;
       char j;
    };
    
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

    // SAMPLE USER DEFINED FUNCTION:
    // checks to see if the integers are equal
    int test_equality_function(void * ut1, void * ut2){
        return !(((struct usr_defined_custom *)ut1)->i != ((struct usr_defined_custom *)ut2)->i || \
        ((struct usr_defined_custom *)ut1)->j != ((struct usr_defined_custom *)ut2)->j);
    }

    
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

    // testing intersection and union
    struct set * ms1 = set_init();
    struct set * ms2 = set_init();
    struct set * result;
    int a,b,c;
    a=1;
    b=2;
    c=3;
    set_add(ms1, &a, INT);
    set_add(ms1, &b, INT);
    set_add(ms2, &c, INT);
    set_add(ms2, &b, INT);
    result = set_union(ms1, ms2);

    // test union
    assert((set_length(result) == 3),FAIL  "set union failed, set should have length=3\n");
    assert(set_member(result,&a,INT),FAIL  "set union failed, set should have INT value=1");
    assert(set_member(result,&b,INT),FAIL  "set union failed, set should have INT value=2");
    assert(set_member(result,&c,INT),FAIL  "set union failed, set should have INT value=3");
    set_free(result);
    printf(PASS "union operation tests passed\n");

    // test intersection
    result = set_intersection(ms1, ms2);
    assert((set_length(result) == 1), FAIL "set intersection failed, set should have length=1\n");
    assert(set_member(result,&b,INT), FAIL "set intersection failed, set should have INT value=3");
    set_free(result);
    printf(PASS "intersection operation tests passed\n");

    // test symmetric difference 
    result = set_symetric_diff(ms1, ms2);
    assert((set_length(result) == 2),FAIL  "set symmetric difference failed, set should have length=2\n");
    assert(set_member(result,&a,INT),FAIL  "set symmetric difference failed, set should have INT value=1");
    assert(set_member(result,&c,INT),FAIL  "set symmetric difference failed, set should have INT value=3");
    set_free(result);
    printf(PASS "symmetric difference operation tests passed\n");
    
    // test complement
    result = set_complement(ms1, ms2);
    assert((set_length(result) == 1), FAIL "set complement failed, set should have length=1\n");
    assert(set_member(result,&a,INT), FAIL "set complement failed, set should have INT value=1");
    printf(PASS "complement operation tests passed\n");

    // test subset
    assert(set_subset(result,ms1), FAIL "set subset failed, result is a subset of ms1\n"); 
    assert(!set_subset(ms2,ms1), FAIL "set subset failed, ms1 is not a subset of ms2\n"); 
    set_free(result);
    printf(PASS "subset operation tests passed\n");

    set_free(ms1);
    set_free(ms2);
    
    int string_equality_function(void *ut1, void * ut2){
        return !strcmp((char *)ut1,(char*)ut2);
    }
    int string_print(void *val){
        printf("%s\n", (char*)val);
	return 0;
    }
    struct set * s8 = set_init();
    struct adt_funcs f;
    f.ptr_equality =  string_equality_function;
    set_add_adt(s8, &f, STRING); 
    char *str1 = "test string1";
    char *str2 = "test string2";
    char *str3 = "test string3";
    printf("set length %d\n",set_length(s8));
    set_add(s8, str1, STRING);
    printf("set length %d\n",set_length(s8));
    set_add(s8, str2, STRING);
    printf("set length %d\n",set_length(s8));
    set_add(s8, str3, STRING);
    printf("set length %d\n",set_length(s8));
    printf("num_adt: %d\n", set_num_adts(s8));
    set_print(s8);
    f.ptr_print =  string_print;
    set_print(s8);

    set_free(s8);
    return 0;
}

