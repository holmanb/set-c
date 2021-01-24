#ifndef UTIL_H
#define UTIL_H
#include <signal.h>
#include <stdlib.h>

#define DEBUG 1

#define checkNull(ptr) assert(ptr, "ERROR: null pointer assertion failed\n")


#define assert(cond, m) \
    if(!(cond)){die("%s",m);}

#define debug(fmt, ...) \
	        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
				                                __LINE__, __func__, __VA_ARGS__); } while (0)

#define die(fmt, ...) \
	        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
				                                __LINE__, __func__, __VA_ARGS__); \
							raise(SIGSEGV);} while (0)



void * xalloc(size_t size);
void * xrealloc(void * p, size_t size);

#endif
