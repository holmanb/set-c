#ifndef UTIL_H
#define UTIL_H
#include <signal.h>

/// best to fail early and often rather than 
/// trying to debug/fix/reproduce bugs that result from mem errors
#define checkNull(ptr) assert(ptr, "Null pointer!! You shall not pass!\n")


#define assert(cond, m) \
    if(!(cond)){die(m);}

#define die(msg) printf("Error thrown %s:%d:\n"msg ,__FILE__, __LINE__);raise(SIGSEGV)

void * xalloc(size_t size);
void * xrealloc(void * p, size_t size);



#endif
