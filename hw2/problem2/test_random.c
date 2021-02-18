#include <stdio.h>
#include <stdlib.h>
#include "myrand.h"
#include "test_random.h"

void test_myrandom(void)
{
    printf("Commencing self built pseudo-random number generator test:\n");
    int var;
    int mycounter[15]={0};

    for (int i=0; i<200000; i++)
    {
        var=(random()%15)+1;
        switch(var)
        {
            case 1:
            mycounter[0]++;
            break;

            case 2:
            mycounter[1]++;
            break;

            case 3:
            mycounter[2]++;
            break;

            case 4:
            mycounter[3]++;
            break;

            case 5:
            mycounter[4]++;
            break;

            case 6:
            mycounter[5]++;
            break;

            case 7:
            mycounter[6]++;
            break;

            case 8:
            mycounter[7]++;
            break;

            case 9:
            mycounter[8]++;
            break;

            case 10:
            mycounter[9]++;
            break;

            case 11:
            mycounter[10]++;
            break;

            case 12:
            mycounter[11]++;
            break;

            case 13:
            mycounter[12]++;
            break;

            case 14:
            mycounter[13]++;
            break;

            case 15:
            mycounter[14]++;
            break;
        }
    }

    printf("Self made pseudo-random number generator test is completed\n");
    for(int j=0; j<15; j++)
    {
        printf("Number %d occured %d times\n",(j+1),mycounter[j]);
    }
}

void test_crandom(void)
{
    printf("Commencing C built in random number generator test:\n");
    int cvar; 
    int ccounter[15]={0};
    time_t t;
    srand((unsigned) time(&t));
    for (int i=0; i<200000; i++)
    {
        cvar=(rand()%15)+1;
        switch(cvar)
        {
            case 1:
            ccounter[0]++;
            break;

            case 2:
            ccounter[1]++;
            break;

            case 3:
            ccounter[2]++;
            break;

            case 4:
            ccounter[3]++;
            break;

            case 5:
            ccounter[4]++;
            break;

            case 6:
            ccounter[5]++;
            break;

            case 7:
            ccounter[6]++;
            break;

            case 8:
            ccounter[7]++;
            break;

            case 9:
            ccounter[8]++;
            break;

            case 10:
            ccounter[9]++;
            break;

            case 11:
            ccounter[10]++;
            break;

            case 12:
            ccounter[11]++;
            break;

            case 13:
            ccounter[12]++;
            break;

            case 14:
            ccounter[13]++;
            break;

            case 15:
            ccounter[14]++;
            break;
        }
    }

    printf("C Built in random number generator test is completed\n");
    for(int j=0; j<15; j++)
    {
        printf("Number %d occured %d times\n",(j+1),ccounter[j]);
    }
}