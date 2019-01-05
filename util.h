
/// best to fail early and often rather than 
/// trying to debug/fix/reproduce bugs that result from mem errors
#define checkNull(ptr) assert(ptr, "Null pointer passed to function! You shall not pass!\n")


#define assert(cond, m) \
    if(!(cond)){die(m);}

#define die(msg) printf(msg);exit(EXIT_FAILURE)



/*simplify and standardize response malloc*/
void * xalloc(ssize_t size){
    void * ptr = malloc(size);
    if(ptr == NULL){
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    } 
    return ptr;
}
