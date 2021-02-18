#ifndef MYRAND_H
#define MYRAND_H

#define a (16807)
#define b (2147483647)
static int next = 1;
int random(void);
void setseed(int seed);

#endif