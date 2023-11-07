#include "rogue/rl_game_loop.h"

#include "rogue/rl_game_context.h"
#include "rogue/io/rl_config.h"

#include "kinc/system.h"
#include "kinc/window.h"
#include "kinc/display.h"

void rl_game_loop_update(void)
{

}

void rl_game_loop_boot(int primary_window)
{
    //NOTE(<zshoals> 11-06-2023):> The entire game context is RIGHT...HERE!~
    //======================================================================
    static rl_game_context_t context = {0};
    //======================================================================

    context.primary_window = primary_window;
    {
        context.config = rl_config_load_from_disk_or_apply_defaults();
        rl_config_save_to_disk(context.config);

        kinc_window_options_t wo = {0};
        {
            wo.x = -1;
            wo.y = -1;
            wo.width = context.config.window_width;
            wo.height = context.config.window_height;
            wo.display_index = kinc_primary_display();
            wo.visible = true;
            wo.window_features = 
                KINC_WINDOW_FEATURE_MINIMIZABLE | 
                KINC_WINDOW_FEATURE_MAXIMIZABLE | 
                KINC_WINDOW_FEATURE_RESIZEABLE;

            wo.mode = (context.config.wants_borderless_fullscreen) ? KINC_WINDOW_MODE_FULLSCREEN : KINC_WINDOW_MODE_WINDOW;
        }

        kinc_framebuffer_options_t fbo = {0};
        {
            fbo.frequency = 60;
            fbo.color_bits = 32;
            fbo.depth_bits = 16;
            fbo.stencil_bits = 8;
            fbo.samples_per_pixel = 1;

            fbo.vertical_sync = context.config.wants_vertical_sync;
        }

        kinc_window_change_framebuffer(context.primary_window, &fbo);
        kinc_window_resize(context.primary_window, wo.width, wo.height);
        kinc_display_mode_t mode = kinc_display_current_mode(kinc_primary_display());
        int window_half_width = (mode.width / 2) - (context.config.window_width / 2);
        int window_half_height = (mode.height / 2) - (context.config.window_height / 2);
        kinc_window_move(context.primary_window, window_half_width, window_half_height);
        kinc_window_change_mode(context.primary_window, wo.mode);
    }


    //NOTE(<zshoals> 11-06-2023):> Begin the main game loop!
    int a = 9999;
    kinc_start();
}

