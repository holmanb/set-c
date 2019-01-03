#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TEST

// This enum is used to define the object type so that 
// a single set can have multiple types in it AND have a 
// coherent way to check equality (check type, if they match, 
// have a mega-switching function to check and cast appropriately
// if all goes well, we might even get a user-defined enum values 
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
    LONG_DOUBLE
} DATA_TYPE;

/*simplify and standardize response malloc*/
void * xalloc(ssize_t size){
    void * ptr = malloc(size);
    if(ptr == NULL){
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    } 
    return ptr;
}

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
};

// store data && keep track of the number of references
struct obj {
    void *data;
    DATA_TYPE type;
    unsigned int ref;
};

// user manipulation
struct obj  * Obj(void *, DATA_TYPE d); 
void   obj_free(struct obj *);
struct set  * set_init(void);
void          set_free(struct set *);
void set_add(struct set *, struct obj *);
void set_delete(struct set *, struct obj *); // TODO
int  set_member(struct set *, struct obj *); 

// allocation
static struct node * node_init(void);
static void node_free(struct node *);
static struct obj  * obj_init(void);

// first-done-next idiom is used for iterating the set w/a for loop
static struct node * set_first(struct set *);
static struct node * set_done(struct set *);
static struct node * set_next(struct set *);

// for use in set_delete() and set_member()
static int obj_equal(struct obj *, struct obj *);

/*for reference counting (most likely removing this)*/
//struct set * set_get(struct set *);
//struct set * set_put(struct set *);

int set_member(struct set *s, struct obj *o){
    struct node * n;
    for(n = set_first(s); set_done(s); n = set_next(s)){
        if(obj_equal(n->obj, o)){
            return 1;    
        }
    }
    return 0;
}


// TODO create a test for this function
struct obj  * Obj(void * v, DATA_TYPE t){
    struct obj * o = obj_init();
    o->data = (void*)v;
    o->type = t;
    return o;
}

int obj_equal(struct obj * o1, struct obj * o2){
    if(o1->type != o2->type){
        return 0;
    }

    // some macro magic would've made this look nicer
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
        default: exit(EXIT_FAILURE);
    }
}

void set_add(struct set *s, struct obj * o) {

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
}

struct obj * obj_init(void){
    struct obj * o = NULL;
    o = (struct obj *) xalloc(sizeof(struct obj));
    o->data = NULL;
    o->ref = 0;
    return o;
}

void obj_free(struct obj * o){
    free(o); 
}

static struct node * node_init(void){
    struct node *new_node=NULL;
    new_node = (struct node *) xalloc(sizeof(struct node));
    new_node->next = NULL;
    new_node->obj= NULL;
    return new_node;
}

static void node_free(struct node * n){
    if(n->obj){
        free(n->obj);
    }
    free(n);
}

struct set * set_init(void){
    struct set *new_set;
    new_set = (struct set *) xalloc(sizeof(struct set));

    new_set->num = 0;
    new_set->head = NULL;
    new_set->iter = NULL;
    new_set->tail = NULL;

    return new_set;
}

void set_free(struct set * s){

    // need to iterate over the nodes in the list and free them individually - write this after set_add()
    struct node * free_node=NULL;

    // iterate and free
    for(free_node = set_first(s); set_done(s); free_node = set_next(s)){
        node_free(free_node);
        free_node = NULL;
    }
    free(s);
}


struct node * set_first(struct set *s){
    s->iter = s->head;
    if(s->head){
        s->iter_next = s->head->next;
    }
    return s->head;
}

//returns true if the list has been completely iterated
struct node * set_done(struct set *s){
    return s->iter?s->iter:NULL; 
}

struct node * set_next(struct set *s){
    s->iter = s->iter_next;
    if(s->iter_next)
        s->iter_next = s->iter_next->next;
    return s->iter;
}



#ifdef TEST

/* 
main()
basic testing of the functionality  
this isn't a test framework, just some basic sanity checking to aid in development
*/

int main(){
        
    struct node test1;
    struct node test2;
    int int_test1 = 1;
    int int_test2 = 512;
    struct obj * obj=NULL;
    obj = (struct obj *) xalloc(sizeof(struct obj));
    test1.obj= obj;

    test1.next = &test2;
    obj->data = &int_test2;
    test2.obj= obj; 
    test2.next = NULL;


    printf("test the node and obj data structures [%d==512]\n", *((int*)test1.obj->data));
    printf("test the node and obj data structures [%d==512]\n", *((int*)test2.obj->data));
    printf("test the node and obj data structures [%d==512]\n", *((int*)test1.next->obj->data));

    struct node *new_node1=NULL;
    struct node *new_node2=NULL;
    new_node1 = (struct node *) xalloc(sizeof(struct node));
    new_node2 = (struct node *) xalloc(sizeof(struct node));
    new_node1->next = new_node2;
    obj->data=&int_test2;
    new_node2->obj = obj; 
    

    printf("test the node functionality: [512==%d]\n",*((int*)new_node1->next->obj->data)); 
    free(new_node1);
    free(new_node2);

    struct node * node1 = NULL;
    struct node * node2 = NULL;
    node1 = node_init();
    node2 = node_init();
    node1->next = node2;
    obj->data =&int_test2;
    node2->obj = obj;

    printf("test the node_init() function: [512==%d]\n", *((int*)node2->obj->data));
    printf("test the node_init() function: [512==%d]\n", *((int*)node1->next->obj->data));

    free(node1);
    free(node2);
    free(obj);

    struct obj * new_obj=NULL;
    struct obj * new_obj1=NULL;
    struct obj * new_obj2=NULL;
    struct obj * new_obj3=NULL;
    struct set * new_set2=NULL;
    struct node * n = NULL;

    int i1 = 1024;
    int i2 = 2048;
    int i3 = 4096;
    int i4 = 4096;

    new_obj = obj_init();
    new_obj1 = obj_init();
    new_obj2 = obj_init();
    new_obj3 = obj_init();

    new_obj->type = INT;
    new_obj1->type = INT;
    new_obj2->type = INT;
    new_obj3->type = INT;

    
    new_obj->data = &i1;
    new_obj1->data = &i2;
    new_obj2->data = &i3;
    new_obj3->data = &i4;

    new_set2 = set_init();

    set_add(new_set2, new_obj);
    set_add(new_set2, new_obj1);
    set_add(new_set2, new_obj2);

    // testing set_member() function
    if(set_member(new_set2, new_obj1)){
        printf("set_member() working properly\n");
    }

    // testing set_member() function
    if(set_member(new_set2, new_obj3)){
        printf("set_member() working properly\n");
    }

    set_add(new_set2, new_obj3);
    printf("new_set2 successfully assigned\n");

    for(n=set_first(new_set2); set_done(new_set2); n = set_next(new_set2)){
        int * i;
        i = (int*) n->obj->data;
        //printf("item: [%d] from ptr %p\n", *(i), n);
    }

    set_free(new_set2);
    
    struct set * new_set1=NULL;
    new_set1 = set_init();
    set_free(new_set1);
    printf("set_init() and set_free() passed\n");

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
    if(!obj_equal(o1, o2)){
        printf("i and j should be equal\n");
        exit(EXIT_FAILURE);
    }
    //same types, diff values
    if(obj_equal(o1, o3)){
        printf("i and k should not be equal\n");
        exit(EXIT_FAILURE);
    }
    //different types,
    if(obj_equal(o3,o4)){
        printf("l and k should not be equal\n");
        exit(EXIT_FAILURE);
    }
    printf("it appears that obj_equal() and Obj() are working properly\n");

    free(o1);
    free(o2);
    free(o3);
    free(o4);
    free(i);
    free(j);
    free(k);
    free(l);

    return 0;
}

#endif
