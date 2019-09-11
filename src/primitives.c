
#include "primitives.h"
//#include INCLUDE_C(PRIMITIVES_TYPE)
//
#define QUOTEME_INNER(M)       #M
#define QUOTEME(M)       QUOTEME_INNER(M)
#define CONCAT3(X, Y, Z) X ## Y ## Z
#define INCLUDE_C(M)  QUOTEME(CONCAT3(M, _, primitives.c))

