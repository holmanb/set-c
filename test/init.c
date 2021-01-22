#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "set.h"

int main(void){
	/*compile/link/init test*/
	struct set * new_set1=NULL;
	new_set1 = set_init();
	set_free(new_set1);
	printf("set_init() and set_free() passed\n");
	return 0;
}

