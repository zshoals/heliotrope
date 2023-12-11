#include "kinc/window.h"
#include "kinc/system.h"
#include "rogue/rl_game_loop.h"

int kickstart(int argc, char** argv) 
{
	//Note(zshoals):> These are basic defaults just to start Kinc. They're almost immediately overwritten by the config file,
	//  which can only be loaded after kinc_init (unfortunately)

	//NOTE(<zshoals> 11-13-2023):> We use this window options dummy to avoid an intermediate window popping up
	//	before we actually configure it. This is solely here for wo_dummy.visible = false
	kinc_window_options_t wo_dummy = {0};
	{
		wo_dummy.x = -1;
		wo_dummy.y = -1;
		wo_dummy.width = 800;
		wo_dummy.height = 600;
		// wo_dummy.display_index = 0; pls work without initializing kinc_display :((((
		wo_dummy.visible = false;
	}

	int primary_window = kinc_init("Heliotrope Fields", 800, 600, &(wo_dummy), NULL);
	rl_game_loop_boot(primary_window);

	return 0;
}

