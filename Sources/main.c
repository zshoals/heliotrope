#include "kinc/window.h"
#include "kinc/system.h"
#include "kinc/display.h"

#include "rogue/rl_common.h"

int kickstart(int argc, char** argv) 
{
	kinc_display_init();

	char const * config_title = "Heliotrope Fields";
	int config_window_width = 800;
	int config_window_height = 600;
	bool config_vsync = true;

	//Initialize window and framebuffer options here with struct inits
	//No crazy construction functions, just do it manually
	//Eventually load settings from config file somewhere
	//NOTE: WE HAVE TO LOAD THESE BEFORE DOING CONFIG STUFF ANYWAY, WE NEED KINC INIT OH NOOOO
	int features = 
		KINC_WINDOW_FEATURE_MINIMIZABLE | 
		KINC_WINDOW_FEATURE_MAXIMIZABLE | 
		KINC_WINDOW_FEATURE_RESIZEABLE;  


	kinc_framebuffer_options_t fbo = {0};
	{
		fbo.color_bits = 32;
		fbo.depth_bits = 16;
		fbo.frequency = 60; //Only useful in exclusive fullscreen mode, sets maximum refresh rate?
		fbo.samples_per_pixel = 1;
		fbo.stencil_bits = 8;
		fbo.vertical_sync = config_vsync;
	}

	kinc_window_options_t wo = {0};
	{
		wo.display_index = kinc_primary_display();
		wo.width = config_window_width;
		wo.height = config_window_height;
		wo.title = config_title;
		wo.visible = true;
		wo.window_features = features;
		wo.x = -1; //Default window position (centered)
		wo.y = -1; //Default window position (centered)
		wo.mode = KINC_WINDOW_MODE_WINDOW;
	}

	kinc_init("Heliotrope Fields", 800, 600, NULL, NULL);

	kinc_start();

	return 0;
}
