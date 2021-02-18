#include <stdio.h>
#include <stdlib.h>
#include "myrand.h"

int random(void)
{
    next = (a * next) % b;
    if(next<1)
        random();
    return next;
}

void setseed(int seed)
{
    next = seed;
}