#include "REF.h"
#include <stdio.h>

int
main(int argc, char const *argv[])
{
  int iN = 0;
  REF_List kRefs;

  REF_Init(&kRefs, &iN);

  int *piN0, *piN1;

  REF_Add(&kRefs, (void **) &piN0);

  REF_Add(&kRefs, (void **) &piN1);

  printf("%d\n", *piN0);
  printf("%d\n", *piN1);

  iN = 1;

  printf("%d\n", *piN0);
  printf("%d\n", *piN1);

  REF_Close(&kRefs);

  printf("%p\n", piN0);
  printf("%p\n", piN1);
}
