# set-c: generic set implementation in C

SUMMARY

	Generic set implementation with support for multi-type sets as well
	as user defined data types (yes, even complex numbers!!)

FUNCTIONS

	set_init, set_free, set_add, set_delete, set_member, set_length, set_union,
	set_complement, set_subset, set_intersection, set_print

METHODOLOGY

	set-c is implemented as a linked list of void pointers, but internal
	data structures should be treated as opaque.  An interface is provided
	for adding user-defined data structures and for manipulating the set 
	data structure.  The interface is very simple.  This README, the 
	example code in test.c, and the functions/data structures exposed in 
	the header file should be enough to get you started.  If additional
	documentation is required, please create an issue on github.

SYNOPSIS

	#include "set.h"

	struct set * set_init(void);

	void set_free(struct set * s);

	int set_add(struct set *s, void *d, DATA_TYPE t);

	int set_delete(struct set * s, void * d, DATA_TYPE t);

	int set_length(struct set * s);

	int set_member(struct set *s, void *d, DATA_TYPE t);

	struct set * set_union(struct set *s1, struct set *s2);

	struct set * set_intersection(struct set *s1, struct set *s2);

	struct set * set_symetric_diff(struct set *s1, struct set *s2);

	struct set * set_complement(struct set *A, struct set *B);

	int set_subset(struct set *A, struct set *B);

	void set_print(struct set *s);

	int  set_add_adt(struct set * s, ptr_equality is_equal, DATA_TYPE dt);  

	struct node * set_first(struct set *);

	struct node * set_done(struct set *);

	struct node * set_next(struct set *);

	void * node_get_data(struct node *n);

	DATA_TYPE node_get_type(struct node *n);


DESCRIPTION

	In this implementation a single set may contain multiple data types (including user 
	defined types). 
	
	Functions set_init, set_member, and set_free allow the caller to create and destroy 
	a set data structure with each node stored as a DATA_TYPE (enum) and value. The 
	programmer should not attempt direct access or modification of the fields in the 
	set data structure; functions for manipulating the set are exposed in the header 
	file.

	Functions set_member, set_add, set_delete, set_length, set_print are basic operations for 
	managing the data structure.  Set-specific operations include set_union, set_intersection,
	set_symetric_diff, set_complement, set_subset.

	User defined data types are supported by this data structure.  To define a custom type,
	add a field to the enum DATA_TYPE for the new type in set.h.  Additionally, a 
	function for comparing equality of the type must be created.  The function set_add_adt 
	is used for registering the custom type.  Look in test.c for an example.
	
