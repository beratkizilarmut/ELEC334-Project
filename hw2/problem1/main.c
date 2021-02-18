#include <stdio.h>
#include <stdlib.h>
#include "myrand.h"

int main()
{
    int x;
    printf("Set the pseudo random number generator seed: ");
    scanf("%d",&x);
    setseed(x);
    for(int i=0; i<10; i++)
    printf("%d\n",(random()%15)+1);
    return 0;
}