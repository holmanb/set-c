#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "set.h"

//#define SIZE 1<<26
#define SIZE 1<<22

int main(void){

	int i;
	struct set * s;
	for (i=0; i<SIZE; i++){
		s = set_init();
		set_free(s);
	}
	return 0;
}

