
//// This enum is used to define the object type so that 
//// a single set can have multiple types in it AND have a 
//// coherent way to check equality (check type, then if they match, 
//// have a mega-switching function to check and cast appropriately
//// if all goes well, we might even add support for ADTs with user-defined enum values 
//// (and a method for registering a compare function for the user defined values) 
//
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

typedef int (*ptr_equality)(void*, void*);

// this will allow the user to pass in an abstract data type
struct usr_type {
    DATA_TYPE type; // the user can add their type to typedef at the end of the list, pass the type to the 
    union {
        ptr_equality type_equal;
    };
};
