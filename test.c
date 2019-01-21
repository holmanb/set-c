#include <stdio.h>
#include <stdlib.h>

#include "util.h" // isn't necessary for user apps
#include "set.h"  // only set.h is necessary for user apps


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
    
    *i1 = 1024;
    *i2 = 2048;
    *i3 = 4096;
    *i4 = 8192;
    *i5 = 8192;


    new_set2 = set_init();

    set_add(new_set2, i1, INT);
    set_add(new_set2, i2, INT);
    set_add(new_set2, i3, INT);

    // testing set_member() function
    //printf("testing if i3 (%d) is a member: \n\t", *i3);
    assert(set_member(new_set2, i3, INT),"set_member() not working properly [1]\n");
        
    // testing set_member() function
    assert(!set_member(new_set2, i4, INT), "set_member() not working properly [2]\n");
    printf("basic set_member() tests passed\n");

    // i4 is equal to i5, so this should fail
    set_add(new_set2, i4, INT);
    int y = set_add(new_set2, i5, INT);
    assert(y,"set_add() didn't deny adding duplicate\n");

    int n=set_length(new_set2);
    
    set_delete(new_set2, i4, INT);
    assert(n-1==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    set_delete(new_set2, i2, INT);
    assert(n-2==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    set_delete(new_set2, i1, INT);
    assert(n-3==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    set_delete(new_set2, i3, INT);
    assert(n-4==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    printf("set_delete() and set_length() tests passed\n");

    set_free(new_set2);
    free(i1);
    free(i2);
    free(i3);
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
    
    set_add_adt(s,&test_equality_function, USER_DEFINED); 

    assert(set_length(s) == 0, "there shouldn't be any items in the set yet\n");
    set_add(s, c1, USER_DEFINED);
    assert(set_length(s) == 1, "there should only be one item in the set \n");
    set_add(s, c2, USER_DEFINED);
    assert(set_length(s) == 1, "there should only be one item in the set \n");
    set_add(s, c3, USER_DEFINED);
    assert(set_length(s) == 2, "there should only be two items in the set \n");
    printf("abstract data type support tests passed\n");
    

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
    assert((set_length(result) == 3), "set union failed, set should have length=3\n");
    assert(set_member(result,&a,INT), "set union failed, set should have INT value=1");
    assert(set_member(result,&b,INT), "set union failed, set should have INT value=2");
    assert(set_member(result,&c,INT), "set union failed, set should have INT value=3");
    set_free(result);

    // test intersection
    result = set_intersection(ms1, ms2);
    assert((set_length(result) == 1), "set intersection failed, set should have length=3\n");
    assert(set_member(result,&b,INT), "set intersection failed, set should have INT value=3");

    printf("union and intersection set operation tests passed\n");

    set_free(ms1);
    set_free(ms2);
    set_free(result);
    return 0;
}

