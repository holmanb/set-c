#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#define COMPILE_TEST 

// This enum is used to define the object type so that 
// a single set can have multiple types in it AND have a 
// coherent way to check equality (check type, then if they match, 
// have a mega-switching function to check and cast appropriately
// if all goes well, we might even add support for ADTs with user-defined enum values 
// (and a method for registering a compare function for the user defined values) 

typedef enum { // TODO: create function to pass in function type and enum for it 
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
    #####################################
    # DO NOT DELETE FIELDS OF THIS ENUM #
    #####################################

    User may add to this enum for an abstract data type. 
    The user must create a function for comparing equality of the data type. 
    This is done using the set_add_adt() function
    */
    USER_DEFINED,
} DATA_TYPE;

struct usr_defined_custom{
   int i;
   char j;
};



// this will allow the user to pass in an abstract data type
struct usr_type {
    DATA_TYPE type; // the user can add their type to typedef at the end of the list, pass the type to the 
    union {
        int (*type_equal)(void *, void *); 
        int (*type_equal_test)(struct usr_defined_custom*, struct usr_defined_custom *);
    };
};


// llnode
struct node {
    struct obj *obj;
    struct node *next;
};

// holds state information of the set
struct set {
    struct node *head;
    struct node *tail;
    struct node *iter;
    struct node *iter_next; // required to free the sll
    unsigned int num;
    DATA_TYPE * dt;
    struct usr_type *custom_types;
    unsigned int num_adts;
};

// store data && keep track of the number of references
struct obj {
    void *data;
    DATA_TYPE type;
    unsigned int ref;
};


//TODO: remove the Obj from the user domain... it is unnecessary - just let the user pass whatever they want to set_add
// user manipulation
struct obj  * Obj(void *, DATA_TYPE d);             // intializes Obj to be added to a set
void   obj_free(struct obj *);                      // frees an Obj (only do this manually if the object isn't added to a set)
                                                    // example: if it holds a duplicate value - objects added to a set are freed by set_delete/set_free
struct set  * set_init(void);                       // initializes set
void          set_free(struct set *);               // frees all items in a set
int set_add(struct set *, struct obj *);            // allocates ll node
int set_delete(struct set *, struct obj *);         // frees ll node
int  set_member(struct set *, struct obj *);        // 1 if obj is a member
int  set_add_adt(struct set *, struct usr_type *);  // used for adding user defined adt functionality 
int  set_length(struct set *);

// internal allocation
static struct node * node_init(void);
static void node_free(struct node *);
static struct obj  * obj_init(void);

// first-done-next idiom is used for iterating the set w/a for loop
static struct node * set_first(struct set *);
static struct node * set_done(struct set *);
static struct node * set_next(struct set *);

// for use in set_delete() and set_member()
static int obj_equal(struct obj *, struct obj *);
static int obj_equal_adt(struct set * s, struct obj *, struct obj *);

// for debugging
static void set_print(struct set *);

/*for reference counting (most likely removing this)*/
//struct set * set_get(struct set *);
//struct set * set_put(struct set *);

int  set_add_adt(struct set * s, struct usr_type *ut){
    
    // increase size of adt pointer 
    s->num_adts += 1;
    s->custom_types = xrealloc(s->custom_types, sizeof(struct usr_type) * (s->num_adts));
    s->custom_types[s->num_adts-1] = *ut;
    return 0;
}

int  set_length(struct set *s){ return s->num; }

static void set_print(struct set *s){
    
    checkNull(s);
    printf("printing the set pointers\n");
    struct node * n;
    int x=0;
    for(n=set_first(s); set_done(s); n = set_next(s)){
        printf("\t%d: from ptr %p\n", x, n);
        x++;
    }
    printf("done printing the set\n");
}

int set_delete(struct set * s, struct obj * o){
    checkNull(s);
    checkNull(o);
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
            return 0;
        }
        last = n;
    }
    // node isn't in the set
    return 1;
}

// iterate the set, check for equality
int set_member(struct set *s, struct obj *o){
    checkNull(s);
    checkNull(o);
    struct node * n;
    for(n = set_first(s); set_done(s); n = set_next(s)){
        if(obj_equal_adt(s,n->obj, o)){
            return 1;    
        }
    }
    return 0;
}


// creates an obj pointer for the user
struct obj  * Obj(void * v, DATA_TYPE t){
    checkNull(v);
    struct obj * o = obj_init();
    o->data = (void*)v;
    o->type = t;
    return o;
}

int obj_equal(struct obj * o1, struct obj * o2){
    checkNull(o2);
    checkNull(o1);
    if(o1->type != o2->type){
        return 0;
    }

    // some macro magic would've made this look cleaner
    // I'm hoping that the choice of clarity is appreciated 
    // by the person reading this
    switch(o1->type){
        case CHAR:          return (*(char *)o1->data == *(char *)o2->data);
        case UCHAR:         return (*(unsigned char *)o1->data == *(char *) o2->data);
        case SHORT:         return (*(short *)o1->data == *(short *) o2->data);
        case USHORT:        return (*(unsigned short*)o1->data == *(unsigned short *) o2->data);
        case INT:           return (*(int *)o1->data == *(int *) o2->data);
        case UINT:          return (*(unsigned int *)o1->data == *(unsigned int *) o2->data);
        case LONG:          return (*(long *)o1->data == *(long *)o2->data);
        case LONG_LONG:     return (*(long long *)o1->data == *(long long *) o2->data);
        case ULONG_LONG:    return (*(unsigned long long *)o1->data == *(long long *) o2->data);
        case ULONG:         return (*(unsigned long *)o1->data == *(unsigned long *) o2->data);
        case FLOAT:         return (*(float *)o1->data == *(float *) o2->data);
        case DOUBLE:        return (*(double *)o1->data == *(double *) o2->data); 
        case LONG_DOUBLE:   return (*(long double *)o1->data == *(long double *) o2->data);
        default: 
        exit(EXIT_FAILURE);
    }
}


int obj_equal_adt(struct set * s, struct obj * o1, struct obj * o2){
    checkNull(s);
    checkNull(o2);
    checkNull(o1);
    if(o1->type != o2->type){
        return 0;
    }

    // some macro magic would've made this look cleaner
    // I'm hoping that the choice of clarity is appreciated 
    // by the person reading this
    switch(o1->type){
        case CHAR:          
        case UCHAR:         
        case SHORT:         
        case USHORT:        
        case INT:           
        case UINT:          
        case LONG:          
        case LONG_LONG:     
        case ULONG_LONG:    
        case ULONG:         
        case FLOAT:         
        case DOUBLE:        
        case LONG_DOUBLE:   
        return obj_equal(o1, o2);
        default: 

        for(int i=0; i < (s->num_adts); i++){
            if(s->custom_types[i].type == o1->type){
                return s->custom_types[i].type_equal(o1->data,o2->data); 
            }
        }
        fprintf(stderr, "Comparasin function not properly assigned for user defined data type\n");
        exit(EXIT_FAILURE);
    }
}

int set_add(struct set *s, struct obj * o) {
    checkNull(s);
    checkNull(o);

    if(set_member(s,o)){ return 1; } // no duplicates in a set

    // create node 
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

struct obj * obj_init(void){
    struct obj * o = NULL;
    o = (struct obj *) xalloc(sizeof(struct obj));
    o->data = NULL;
    o->ref = 0;
    return o;
}

void obj_free(struct obj * o){
    checkNull(o);
    free(o); 
    o=NULL;
}

static struct node * node_init(void){
    struct node *new_node=NULL;
    new_node = (struct node *) xalloc(sizeof(struct node));
    new_node->next = NULL;
    new_node->obj= NULL;
    return new_node;
}

static void node_free(struct node * n){
    checkNull(n);
    if(n->obj){
        obj_free(n->obj);
    }
    free(n);
    n=NULL;
}

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


struct node * set_first(struct set *s){
    checkNull(s);
    s->iter = s->head;
    if(s->head){
        s->iter_next = s->head->next;
    }
    return s->head;
}

//returns true if the list has been completely iterated
struct node * set_done(struct set *s){
    checkNull(s);
    return s->iter?s->iter:NULL; 
}

struct node * set_next(struct set *s){
    s->iter = s->iter_next;
    if(s->iter_next)
        s->iter_next = s->iter_next->next;
    return s->iter;
}



#ifdef COMPILE_TEST 

/* 
main()
basic testing of the functionality  
this isn't a test framework, just some basic sanity checking to aid in development
*/

int main(){
        
    struct node test1;
    struct node test2;
    int int_test2 = 512;
    struct obj * obj=NULL;
    obj = (struct obj *) xalloc(sizeof(struct obj));
    test1.obj= obj;

    test1.next = &test2;
    obj->data = &int_test2;
    test2.obj= obj; 
    test2.next = NULL;


    assert((*((int*)test1.obj->data)==512),"struct obj test failed\n");
    assert(*((int*)test2.obj->data),"struct obj test failed\n");
    assert(*((int*)test1.next->obj->data),"struct obj test failed\n");
    printf("basic obj struct tests passed\n");

    struct node *new_node1=NULL;
    struct node *new_node2=NULL;
    new_node1 = (struct node *) xalloc(sizeof(struct node));
    new_node2 = (struct node *) xalloc(sizeof(struct node));
    new_node1->next = new_node2;
    obj->data=&int_test2;
    new_node2->obj = obj; 
    

    assert((*(int*)new_node1->next->obj->data)==512,"test the node struct functionality: \n"); 
    free(new_node1);
    free(new_node2);
    printf("basic node struct tests passed\n");

    struct node * node1 = NULL;
    struct node * node2 = NULL;
    node1 = node_init();
    node2 = node_init();
    node1->next = node2;
    obj->data =&int_test2;
    node2->obj = obj;

    assert(512==*((int*)node2->obj->data),"the node_init() function failed\n");
    assert(512==*((int*)node1->next->obj->data),"the node_init() function failed\n");
    printf("basic node_init() tests passed\n");

    node_free(node1);
    node_free(node2);

    struct set * new_set1=NULL;
    new_set1 = set_init();
    set_free(new_set1);
    printf("set_init() and set_free() passed\n");

    struct obj * new_obj=NULL;
    struct obj * new_obj1=NULL;
    struct obj * new_obj2=NULL;
    struct obj * new_obj3=NULL;
    struct obj * new_obj4=NULL;
    struct set * new_set2=NULL;

    int i1 = 1024;
    int i2 = 2048;
    int i3 = 4096;
    int i4 = 8192;
    int i5 = 8192;

    new_obj = obj_init();
    new_obj1 = obj_init();
    new_obj2 = obj_init();
    new_obj3 = obj_init();
    new_obj4 = obj_init();

    new_obj->type = INT;
    new_obj1->type = INT;
    new_obj2->type = INT;
    new_obj3->type = INT;
    new_obj4->type = INT;

    
    new_obj->data = &i1;
    new_obj1->data = &i2;
    new_obj2->data = &i3;
    new_obj3->data = &i4;
    new_obj4->data = &i5;

    new_set2 = set_init();

    set_add(new_set2, new_obj);
    set_add(new_set2, new_obj1);
    set_add(new_set2, new_obj2);

    // testing set_member() function
    assert(set_member(new_set2, new_obj1),"set_member() not working properly\n");
        
    // testing set_member() function
    assert(!set_member(new_set2, new_obj3) ,"set_member() not working properly\n");
    printf("basic set_member() tests passed\n");

    set_add(new_set2, new_obj3);
    assert(set_add(new_set2, new_obj4),"set_add() didn't deny adding duplicate\n");
    obj_free(new_obj4);
    printf("set_add() test passed: properly denied adding duplicate \n\tnote: since \
the duplicate wasn't added to the set,\n\t it must be manually freed, hence the return code)\n");

    int n=set_length(new_set2);
    
    set_print(new_set2);
    
    set_delete(new_set2, new_obj3);
    assert(n-1==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    set_delete(new_set2, new_obj1);
    assert(n-2==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    set_delete(new_set2, new_obj);
    assert(n-3==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    set_delete(new_set2, new_obj2);
    assert(n-4==(set_length(new_set2)), "set_delete() or set_length() is broken()\n");

    printf("set_delete() and set_length() tests passed\n");

    set_free(new_set2);
    

    // test Obj() creation function
    int *i, *j, *k;
    long *l;
    i=xalloc(sizeof(int));
    j=xalloc(sizeof(int));
    k=xalloc(sizeof(int));
    l=xalloc(sizeof(long));
    *i=0;
    *j=0;
    *k=100;
    *l=100.0;

    struct obj * o1; 
    struct obj * o2; 
    struct obj * o3; 
    struct obj * o4; 
    o1 = Obj(i, INT);
    o2 = Obj(j,INT);
    o3 = Obj(l,LONG);
    o4 = Obj(k,INT);

    //same types, same values
    assert(obj_equal(o1, o2),"i and j should be equal\n");

    //same types, diff values
    assert(!obj_equal(o1, o3),"i and k should not be equal\n");

    //different types,
    assert(!obj_equal(o3,o4),"l and k should not be equal\n");

    printf("obj_equal() and Obj() tests passed\n");

    free(o1);
    free(o2);
    free(o3);
    free(o4);
    free(i);
    free(j);
    free(k);
    free(l);

    struct set * s = NULL;
    s = set_init();
    struct usr_defined_custom{
       int i;
       char j;
    };
    
    struct usr_defined_custom *c1,*c2;
    c1 = xalloc(sizeof(struct usr_defined_custom));
    c2 = xalloc(sizeof(struct usr_defined_custom));

    c1->i=0;
    c1->j='c';

    c2->i=0;
    c2->j='c';

    struct obj *O1, *O2; 
    O1 = Obj(c1,USER_DEFINED);
    O2 = Obj(c2,USER_DEFINED);


    // checks to see if the first element's integer is bigger
    int test_equality_function(void * ut1, void * ut2){
        if(((struct usr_defined_custom *)ut1)->i != ((struct usr_defined_custom *)ut2)->i || ((struct usr_defined_custom *)ut1)->j != ((struct usr_defined_custom *)ut2)->j){
            return 0;
        }
        return 1;
    }


    struct usr_type * ut0 = xalloc(sizeof(struct usr_type));
    ut0->type_equal = &test_equality_function;


    ut0->type = USER_DEFINED;
    set_add_adt(s,ut0); 

    printf("there are currently %d items in set\n", set_length(s));
    set_add(s, O1);
    printf("there are currently %d items in set\n", set_length(s));
    set_add(s, O2);
    printf("there are currently %d items in set\n", set_length(s));

    free(c1);
    free(c2);
    free(ut0);
    free(O2);
    
    set_free(s);
    return 0;
}

#endif
