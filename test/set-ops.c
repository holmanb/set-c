#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "set.h"

#define PASS "PASS - "
#define FAIL "FAIL - "
#define die(msg) printf("Error thrown %s:%d:\n"msg ,__FILE__, __LINE__);raise(SIGSEGV)
#define assert(cond, m) \
    if(!(cond)){die(m);}

int main(void){


	// testing intersection and union
	struct set * ms1 = set_init();
	struct set * ms2 = set_init();
	struct set * result;
	int a,b,c;
	a=1;
	b=2;
	c=3;
	set_add(ms1, &a, INT);
	set_add(ms1, &b, INT);
	set_add(ms2, &c, INT);
	set_add(ms2, &b, INT);
	result = set_union(ms1, ms2);

	// test union
	assert((set_length(result) == 3),FAIL  "set union failed, set should have length=3\n");
	assert(set_member(result,&a,INT),FAIL  "set union failed, set should have INT value=1");
	assert(set_member(result,&b,INT),FAIL  "set union failed, set should have INT value=2");
	assert(set_member(result,&c,INT),FAIL  "set union failed, set should have INT value=3");
	set_free(result);
	printf(PASS "union operation tests passed\n");

	// test intersection
	result = set_intersection(ms1, ms2);
	assert((set_length(result) == 1), FAIL "set intersection failed, set should have length=1\n");
	assert(set_member(result,&b,INT), FAIL "set intersection failed, set should have INT value=3");
	set_free(result);
	printf(PASS "intersection operation tests passed\n");

	// test symmetric difference
	result = set_symetric_diff(ms1, ms2);
	assert((set_length(result) == 2),FAIL  "set symmetric difference failed, set should have length=2\n");
	assert(set_member(result,&a,INT),FAIL  "set symmetric difference failed, set should have INT value=1");
	assert(set_member(result,&c,INT),FAIL  "set symmetric difference failed, set should have INT value=3");
	set_free(result);
	printf(PASS "symmetric difference operation tests passed\n");

	// test complement
	result = set_complement(ms1, ms2);
	assert((set_length(result) == 1), FAIL "set complement failed, set should have length=1\n");
	assert(set_member(result,&a,INT), FAIL "set complement failed, set should have INT value=1");
	printf(PASS "complement operation tests passed\n");

	// test subset
	assert(set_subset(result,ms1), FAIL "set subset failed, result is a subset of ms1\n");
	assert(!set_subset(ms2,ms1), FAIL "set subset failed, ms1 is not a subset of ms2\n");
	set_free(result);
	printf(PASS "subset operation tests passed\n");

	set_free(ms1);
	set_free(ms2);

	return 0;
}

