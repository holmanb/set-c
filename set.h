#ifndef SET_H
#define SET_H

// forward declare to hide implementation
struct set;
struct node;


// This enum is used to define the object type so that 
// a single set can have multiple types in it AND support
// abstract data types in a simple manner
typedef enum {
    CHAR,
    UCHAR,
    SHORT,
    USHORT,
    INT,
    UINT,
    LONG,
    ULONG,
    LONG_LONG,
    ULONG_LONG,
    FLOAT,
    DOUBLE,
    UDOUBLE,
    LONG_DOUBLE,
    /*
    ###########################################
    # DO NOT DELETE ABOVE FIELDS OF THIS ENUM #
    ###########################################

    User may add to this enum for a user defined abstract data type. 
    The user must create a function for comparing equality of the data type. 
    This is done using the set_add_adt() function
    */
    USER_DEFINED,  // see example code in test.c
} DATA_TYPE;


// abstract data types are supported with a
// user defined equality function passed along 
// with a custom DATA_TYPE to set_add_adt()

// equality function for user defined data type
typedef int (*ptr_equality)(void*, void*);


// add user defined data types via this function
int  set_add_adt(struct set * s, ptr_equality is_equal, DATA_TYPE dt);  // note: this may change to support more than just an equality function


// malloc and free
struct set * set_init(void);
void set_free(struct set * s);


// create and destroy values in set
int set_add(struct set *s, void *d, DATA_TYPE t);
int set_delete(struct set * s, void * d, DATA_TYPE t);


// get length of set
int set_length(struct set * s);


// check if value is in set already
int set_member(struct set *s, void *d, DATA_TYPE t);

// returns a set that is the union of the two arguments
struct set * set_union(struct set *s1, struct set *s2);

// returns a set that is the intersection of the two arguments
struct set * set_intersection(struct set *s1, struct set *s2);

// returns a set that is the symetric difference of the two arguments
struct set * set_symetric_diff(struct set *s1, struct set *s2);

// returns a set that is the complement of A given B (A-B)
struct set * set_complement(struct set *A, struct set *B);

// returns true if A is a subset of B 
int set_subset(struct set *A, struct set *B);


// iterate a set: first-done-next idiom is implemented for use with for loop 
// example:
//    struct node *n;
//    for(n=set_first(s); set_done(s); n = set_next(s)){
struct node * set_first(struct set *);
struct node * set_done(struct set *);
struct node * set_next(struct set *);
void * node_get_data(struct node *n);
DATA_TYPE node_get_type(struct node *n);

// prints value of items in the set
// TODO: implement printing user data types
void set_print(struct set *s);

#endif
