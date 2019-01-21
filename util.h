#ifndef UTIL_H
#define UTIL_H
#include <signal.h>

/// best to fail early and often rather than 
/// trying to debug/fix/reproduce bugs that result from mem errors
#define checkNull(ptr) assert(ptr, "Null pointer passed to function! You shall not pass!\n")


#define assert(cond, m) \
    if(!(cond)){die(m);}

#define die(msg) printf("Error thrown:\n"msg);raise(SIGSEGV)

void * xalloc(size_t size);
void * xrealloc(void * p, size_t size);



#endif
