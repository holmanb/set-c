/*******************************
* Title: Set-C
* Author: Brett Holman
* Dates: December 2018-January 2019
* Summary:
* Presents a collection of functions for basic manipulating of a set data structure.
* Note:  This project drew some inspiration initially from some documentation of kobjects, ksets, and ktypes
* by Greg Kroah-Hartman (based on an artical by Jon Corbet). This doesn't intend to relate to kernel data 
* structures in any way.
* https://www.kernel.org/doc/Documentation/kobject.txt
* TODO:
*   1. implement more set operations (difference, is_subset, is_superset)
*   2. implement interface such that user may define print statement
*   3. write test to verify intersection and union
*   4. implement set_pop - THINK ABOUT THIS - should we really have the iterator exposed?
*   what if someone does something that modifies the set (add or remove a node), in the middle of iterating?
*   maybe just remove the iterators and rely on pop instead? or should we do a bunch of testing and verify 
*   that nothing will break?
********************************/


#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "set.h"


// llnode
struct node {
    struct obj *obj;
    struct node *next;
};


// store data && keep track of the number of references
struct obj {
    void *data;
    DATA_TYPE type;
};


// stores info about the user's abstract data type
struct usr_type {
    DATA_TYPE type; // User may add their own enum to the DATA_TYPE
    ptr_equality type_equal;
    ptr_print type_print;
};


// internal allocation
static struct node * node_init(void);
static void node_free(struct node *);
static struct obj  * obj_init(void);
static struct obj  * Obj(void *, const DATA_TYPE d);             // intializes Obj to be added to a set
static void   obj_free(struct obj *);                      // frees an Obj (only do this manually if the object isn't added to a set)


// for use in set_delete() and set_member()
static int obj_equal_adt(struct set * s, const struct obj *, const struct obj *);


// for debugging
static void print_type(struct set *, const struct node *);


// get the data in a node
void * node_get_data(const struct node *n){
    return (void *)n->obj->data;
}


// get the node type
DATA_TYPE node_get_type(const struct node *n){
    return n->obj->type;
}


// returns true if A is a subset of B
int set_subset(struct set *A, struct set *B){
    struct node *n;
    for(n=set_first(A); set_done(A); n = set_next(A)){
        if(!set_member(B, n->obj->data, n->obj->type)){
            return 0;
        }
    }
    return 1;
}


// user operation on two sets - must free returned set
struct set * set_complement(struct set *A, struct set *B){
    struct set *s = set_init();
    struct node *n;
    for(n=set_first(A); set_done(A); n = set_next(A)){
        if(!set_member(B, n->obj->data, n->obj->type)){
            set_add(s,n->obj->data, n->obj->type);
        }
    }
    return s;
}


// user operation on two sets - must free returned set
struct set * set_symetric_diff(struct set *s1, struct set *s2){
    struct set *s = set_init();
    struct node *n;
    for(n=set_first(s1); set_done(s1); n = set_next(s1)){
        if(!set_member(s2, n->obj->data, n->obj->type)){
            set_add(s,n->obj->data, n->obj->type);
        }
    }
    for(n=set_first(s2); set_done(s2); n = set_next(s2)){
        if(!set_member(s1, n->obj->data, n->obj->type)){
            set_add(s,n->obj->data, n->obj->type);
        }
    }
    return s;

}


// user operation on two sets - must free returned set
struct set * set_union(struct set *s1, struct set *s2){
    struct set *s = set_init();
    struct node *n;
    for(n=set_first(s1); set_done(s1); n = set_next(s1)){
        set_add(s,n->obj->data, n->obj->type);
    }
    for(n=set_first(s2); set_done(s2); n = set_next(s2)){
        set_add(s,n->obj->data, n->obj->type);
    }
    return s;
}


// user operation on two sets - must free returned set
struct set * set_intersection(struct set *s1, struct set *s2){
    struct set * s = set_init();
    struct node *n;
    for(n=set_first(s1); set_done(s1); n = set_next(s1)){
        if(set_member(s2,n->obj->data, n->obj->type)){
            set_add(s,n->obj->data, n->obj->type);
        }
    }
    return s;
}


// add an abstract data type (no error checking is done here)
int set_add_adt(struct set * s, const struct adt_funcs *f, const DATA_TYPE dt){
    checkNull(s);
    checkNull(f);
    checkNull(dt);

    // increase size of adt pointer
    s->custom_types = xrealloc(s->custom_types, sizeof(struct usr_type) * (s->num_adts + 1));

    // set values
    s->custom_types[s->num_adts].type = dt;
    s->custom_types[s->num_adts].type_equal = f->ptr_equality;
    s->custom_types[s->num_adts].type_print = f->ptr_print;
    s->num_adts += 1;

    return 0;
}


// get the length of a set
unsigned int set_length(const struct set *s){
    checkNull(s);
    return s->num;
}


// print value of non-adts (should this support adts in the future?)
static void print_type(struct set *s, const struct node * n){
    checkNull(s);
    checkNull(n);
    checkNull(n->obj);
    checkNull(n->obj->data);
    if(!n->obj->data){
        printf("no data found while printing object data\n");
        return;
    }
    switch(n->obj->type){
        case CHAR:          printf("char %c\n",(*(char *)n->obj->data)); break;
        case UCHAR:         printf("unsigned char %c\n",(*(unsigned char *)n->obj->data)); break;
        case SHORT:         printf("short %d\n",(*(short *)n->obj->data)); break;
        case USHORT:        printf("unsigned short %u\n",(*(unsigned short *)n->obj->data)); break;
        case INT:           printf("int %d\n",(*(int *)n->obj->data)); break;
        case UINT:          printf("unsigned int %u\n",(*(unsigned int*)n->obj->data)); break;
        case LONG:          printf("long %ld\n",(*(long *)n->obj->data)); break;
        case LONG_LONG:     printf("long long %lld\n",(*(long long *)n->obj->data)); break;
        case ULONG_LONG:    printf("unsigned long long %lld\n",(*(unsigned long long*)n->obj->data)); break;
        case ULONG:         printf("unsigned long %ld\n",(*(unsigned long *)n->obj->data)); break;
        case FLOAT:         printf("float %f\n",(*(float *)n->obj->data)); break;
        case DOUBLE:        printf("double %f\n",(*(double *)n->obj->data)); break;
        case LONG_DOUBLE:   printf("long double %Lf\n",(*(long double *)n->obj->data)); break;
        default:

            // print adt
            for(unsigned int i=0; i < (s->num_adts); i++){
                if(s->custom_types[i].type == n->obj->type){

                // if the function ptr is null
                if(!s->custom_types[i].type_print) { break; }
    	        s->custom_types[i].type_print(n->obj->data);
    	        return;
            }
        }
        // need to add support for printing custom type
        printf("custom type at address %p (custom type print function is undefined)\n",n->obj->data);
    }
}


// print all values in the set
void set_print(struct set *s){

    checkNull(s);
    struct node * n;
    int x=0;
    for(n=set_first(s); set_done(s); n = set_next(s)){
        printf("\t[%d]: from ptr %p: value=", x, (void *)n);
        print_type(s, n);
        x++;
    }
}


// delete value from the set
int set_delete(struct set * s, void * d, DATA_TYPE t){
    checkNull(s);
    checkNull(d);
    checkNull(t);
    struct obj * o = Obj(d,t);
    struct node *n, *del, *last=NULL;
    for(n = set_first(s); set_done(s); n = set_next(s)){

        // check if set contains the node to be removed
        if(obj_equal_adt(s, n->obj, o)){
            del=n;

            // going fr/1 node in set to empty set
            if(del==s->head && del==s->tail){
                s->head = del->next;
                s->tail = last;

                // can't set next if it's a null ptr (aka empty set)
                if(s->tail){
                    s->tail->next = NULL;
                }

            // node to remove is head node
            } else if(del==s->head){
                s->head = del->next;
            }

            // node to remove is tail node
            else if(del==s->tail){
                s->tail = last;

                // can't set next if it's a null ptr (aka empty set)
                if(s->tail){
                    s->tail->next = NULL;
                }

            }
            else {
                last->next = del->next;
            }

            // free the node
            node_free(del);
            s->num--;
            obj_free(o);
            return 0;
        }
        last = n;
    }
    // node isn't in the set
    obj_free(o);
    return 1;
}


// check if value is contained in the set
int set_member(struct set *s, void *d, const DATA_TYPE t){
    checkNull(s);
    checkNull(d);
    checkNull(t);
    struct obj * o = Obj(d,t);
    struct node * n;
    for(n = set_first(s); set_done(s); n = set_next(s)){
        if(obj_equal_adt(s,n->obj, o)){
            obj_free(o);
            return 1;
        }
    }
    obj_free(o);
    return 0;
}


// creates an obj pointer
struct obj  * Obj(void * v, const DATA_TYPE t){
    checkNull(v);
    checkNull(t);
    struct obj * o = obj_init();
    o->data = v;
    o->type = t;
    return o;
}


// check if two values in set are equal (supports abstract data type comparasin)
int obj_equal_adt(struct set * s, const struct obj * o1, const struct obj * o2){
	checkNull(s);
	checkNull(o2);
	checkNull(o1);
	checkNull(o1->data);
	checkNull(o2->data);
	checkNull(o2->type);
	checkNull(o1->type);
	if(o1->type != o2->type){
		return 0;
	}

	switch(o1->type){
	case CHAR:          return (*(char *)o1->data == *(char *)o2->data);
	case UCHAR:         return (*(unsigned char *)o1->data == *(char *) o2->data);
	case SHORT:         return (*(short *)o1->data == *(short *) o2->data);
	case USHORT:        return (*(unsigned short*)o1->data == *(unsigned short *) o2->data);
	case INT:           return (*(int *)o1->data == *(int *) o2->data);
	case UINT:          return (*(unsigned int *)o1->data == *(unsigned int *) o2->data);
	case LONG:          return (*(long *)o1->data == *(long *)o2->data);
	case LONG_LONG:     return (*(long long *)o1->data == *(long long *) o2->data);
	case ULONG_LONG:    return (*(unsigned long long *)o1->data == *(unsigned long long *) o2->data);
	case ULONG:         return (*(unsigned long *)o1->data == *(unsigned long *) o2->data);
	case FLOAT:         return (*(float *)o1->data == *(float *) o2->data);
	case DOUBLE:        return (*(double *)o1->data == *(double *) o2->data);
	case LONG_DOUBLE:   return (*(long double *)o1->data == *(long double *) o2->data);
	default:

	for(unsigned int i=0; i < (s->num_adts); i++){
		if(s->custom_types[i].type == o1->type){
			return s->custom_types[i].type_equal(o1->data,o2->data);
		}
        }
	die("%s", "abstract data type improperly initialized\n");
	exit(EXIT_FAILURE);
    }
}


// add value to set
int set_add(struct set *s, void *d, const DATA_TYPE t) {
    checkNull(s);
    checkNull(d);
    checkNull(t);

    if(set_member(s, d, t)){ return 1; } // no duplicates in a set

    // create node
    struct obj * o = Obj(d,t);
    struct node * new_node = node_init();
    new_node->obj = o;
    s->num += 1;

    //add to list
    if(s->head == NULL){
        s->tail = new_node;
        s->head = new_node;
    } else {
        s->tail->next = new_node;
        s->tail = new_node;
    }
    return 0;
}


// initialize object (must be freed)
struct obj * obj_init(void){
    struct obj * o = NULL;
    o = (struct obj *) xalloc(sizeof(struct obj));
    o->data = NULL;
    return o;
}


// free obj
void obj_free(struct obj * o){
    checkNull(o);
    free(o);
    o=NULL;
}


// initialize llnode (must be freed)
static struct node * node_init(void){
    struct node *new_node=NULL;
    new_node = (struct node *) xalloc(sizeof(struct node));
    new_node->next = NULL;
    new_node->obj= NULL;
    return new_node;
}


// free node and object
static void node_free(struct node * n){
    checkNull(n);
    if(n->obj){
        obj_free(n->obj);
    }
    free(n);
    n=NULL;
}


// initialize the set to sane values
struct set * set_init(void){
    struct set *new_set;
    new_set = (struct set *) xalloc(sizeof(struct set));

    new_set->num = 0;
    new_set->head = NULL;
    new_set->iter = NULL;
    new_set->iter_next = NULL;
    new_set->tail = NULL;

    new_set->dt = NULL;
    new_set->num_adts = 0;
    new_set->custom_types = NULL;

    return new_set;
}


// free the set and all of its items
void set_free(struct set * s){
    checkNull(s);

    // need to iterate over the nodes in the list and free them individually - write this after set_add()
    struct node * free_node=NULL;

    // iterate and free
    for(free_node = set_first(s); set_done(s); free_node = set_next(s)){
        node_free(free_node);
    }
    free(s->custom_types);
    free(s);
    s=NULL;
}


// get first item in set (for looping)
struct node * set_first(struct set *s){
    checkNull(s);
    s->iter = s->head;
    if(s->head){
        s->iter_next = s->head->next;
    }
    return s->head;
}


// returns true if the list has been completely iterated (for looping)
struct node * set_done(struct set *s){
    checkNull(s);
    return s->iter?s->iter:NULL;
}


// return next item in set (for looping)
struct node * set_next(struct set *s){
    checkNull(s);
    s->iter = s->iter_next;
    if(s->iter_next)
        s->iter_next = s->iter_next->next;
    return s->iter;
}


// returns the number of adts loaded into the set
unsigned int set_num_adts(const struct set *s){
    return s->num_adts;
}
