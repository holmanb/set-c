# set-c

generic set supporting abstract data types in c

# Example

```
#include <stdio.h>
#include "set.h"

void print_membership(struct set *s, int *i){
    printf("%d is%sa member of the set\n",
        *i,
        set_member(s, i, INT) ? " " : " not "
        );
}
int main(void){

    struct set * my_set;

    int i0=0, i1=1, i2=2;
    my_set = set_init();

    set_add(my_set, &i0, INT);
    set_add(my_set, &i1, INT);

    print_membership(my_set, &i0);
    print_membership(my_set, &i1);
    print_membership(my_set, &i2);

    set_free(my_set);
}
```

## Output

```
$ ./example
0 is a member of the set
1 is a member of the set
2 is not a member of the set
```

# Dependencies

- c compiler
- ninja
- meson

# Build

```
$ meson build && cd build
$ ninja
```

# Test

```
$ meson test
```

# Install

```
$ meson install
```

