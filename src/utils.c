#include <stdlib.h>

#include "utils.h"

/*
 * Source: https://github.com/tsoding/sowon/blob/master/png2c.c
 *
 * Thanks @Tsoding for this wonderful idea :))
 */ 
const char* shift(int* argc, char*** argv)
{
    if (*argc <= 0) return NULL;
    const char* result = *argv[0];
    *argc -= 1;
    *argv += 1;
    return result;
}

