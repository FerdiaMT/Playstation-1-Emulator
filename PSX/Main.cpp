#include "PSX.h"
#include <cstdint>
int main()
{
	PSX psx;

	

	

	bool running = true;
	while (running)
	{
		for (int i = 0; i < 10000; i += 1) // EACH TICK CAUSES 2 CYCLES OF EVERYTHING
		{
			psx.tick();
		}

		//updateframe




		running = false; // this is just for debugging
	}

	return 1;

}