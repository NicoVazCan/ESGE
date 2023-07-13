#include "ESGE_hash.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	if (argc > 1)
	{
		printf("%lX\n", ESGE_Hash(argv[1]));
	}
	return 0;
}