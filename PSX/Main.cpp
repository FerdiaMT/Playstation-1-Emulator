#include "PSX.h"
#include <cstdint>
int main()
{
	PSX psx;

	

	

	bool running = true;
	while (running)
	{
		for (int i = 0; i < 10000000; i += psx.cpu.cycles) // cpu cycles is hardcoded on 1 for now
		{
			psx.tick();
		}

		running = false; // this is just for debugging
	}

	return 1;

}