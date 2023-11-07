#include "kinc/window.h"
#include "kinc/system.h"
#include "rogue/rl_game_loop.h"

int kickstart(int argc, char** argv) 
{
	//Note(zshoals):> These are basic defaults just to start Kinc. They're almost immediately overwritten by the config file,
	//  which can only be loaded after kinc_init (unfortunately)
	int primary_window = kinc_init("Heliotrope Fields", 800, 600, NULL, NULL);
	rl_game_loop_boot(primary_window);

	return 0;
}
