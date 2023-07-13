#include <SDL2/SDL.h>
#include "ESGE_error.h"

void Test(void)
{
	ESGE_Error("%s", "lol");
}

int main(int argc, char const *argv[])
{
	Test();

	while(1);

	return 0;
}