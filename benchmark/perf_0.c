#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "set.h"

#define SIZE 2000

/* tests demonstrate that performance is dominated by membership lookups
 * consider a different datastructure for lookup performance (maybe bst?)
 */
int main(void){

	int i;
	struct set * s9 = set_init();
	int ints[SIZE];
	for (i=0; i<SIZE; i++){
		ints[i] = i;
		set_add(s9, &ints[i], INT);
	}
	printf("set length %d\n", set_length(s9));
	set_print(s9);
	set_free(s9);

	return 0;
}

