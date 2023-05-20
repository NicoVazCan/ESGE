#include "PLC.h"
#include <stdio.h>

int mem[] = {0,1,2,3};

#define CB0_PRI 0u
void cb0(PLC_PriGrps **pL, void *d)
{
    int *pN = (int*)d;

    printf("cb0: %d\n", *pN);

    if (++(*pN) == 4)
        PLC_remove(pL, CB0_PRI, cb0, d);
}

#define CB1_PRI CB0_PRI+1u
void cb1(PLC_PriGrps **pL, void *d)
{
    int *pN = (int*)d;

    *pN = mem[0]+mem[1];

    printf("cb1: %d\n", *pN);

    if (*pN < 4)
        PLC_remove(pL, CB1_PRI, cb1, d);
}

int main(int argc, char const *argv[])
{
    PLC_PriGrps *l;

    PLC_init(&l);
    PLC_add(&l, CB0_PRI, cb0, mem);
    PLC_add(&l, CB0_PRI, cb0, mem+1);
    PLC_add(&l, CB1_PRI, cb1, mem+2);
    PLC_add(&l, CB1_PRI, cb1, mem+3);

    while (!PLC_empty(l)) PLC_update(&l);

    return 0;
}