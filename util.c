#include <stdlib.h>
#include <stdio.h>
#include "util.h"


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
