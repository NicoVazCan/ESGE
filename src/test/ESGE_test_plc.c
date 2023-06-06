#include "ESGE_plc.h"
#include <SDL2/SDL.h>


int commonData[4] = {0,0,0,0};

#define PLC0_PRI 0
void PLC0(void *pvN)
{
	int *piN = pvN;

	SDL_Log("PLC0: %d\n", *piN);

	if (++(*piN) > 4) ESGE_RemovePLC(PLC0_PRI, PLC0, pvN);
}

#define PLC1_PRI 0
void PLC1(void *pvN)
{
	int *piN = pvN;

	SDL_Log("PLC1: %d\n", *piN);

	if (++(*piN) > 8) ESGE_RemovePLC(PLC1_PRI, PLC1, pvN);
}

#define PLC2_PRI PLC1_PRI+1
void PLC2(void *pvN)
{
	int *piN = pvN;

	SDL_Log("PLC2: %d\n", *piN);

	if (commonData[1] > 2)
	{
		if (++(*piN) > 4) ESGE_RemovePLC(PLC2_PRI, PLC2, pvN);
	}
}

#define PLC3_PRI PLC2_PRI+1
void PLC3(void *pvN)
{
	int *piN = pvN;

	if (commonData[1] > 2)
	{
		if (++(*piN) > 8) ESGE_DelPLC(PLC3_PRI, PLC3, pvN);
	}
}

int main(SDL_UNUSED int argc, SDL_UNUSED char const *argv[])
{
	ESGE_AddPLC(PLC0_PRI, PLC0, commonData);
	ESGE_AddPLC(PLC1_PRI, PLC1, commonData+1);
	ESGE_AddPLC(PLC2_PRI, PLC2, commonData+2);
	ESGE_AddPLC(PLC3_PRI, PLC3, commonData+3);

	while (ESGE_AnyPLC()) ESGE_UpdatePLC();

	return 0;
}