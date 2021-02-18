#include <stdio.h>
#include <stdlib.h>
#include "myrand.h"
#include "test_random.h"

int main()
{
    int x;
    printf("Set the self built pseudo random number generator seed: ");
    scanf("%d",&x);
    setseed(x);
    test_myrandom();
    test_crandom();
    return 0;
}