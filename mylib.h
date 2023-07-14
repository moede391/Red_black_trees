#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>
#include "tree.h"

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern void swap(rbt_colour xp, rbt_colour yp);

#endif
