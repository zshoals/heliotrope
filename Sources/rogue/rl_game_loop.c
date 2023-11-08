#include "rogue/rl_game_loop.h"

#include "rogue/rl_game_context.h"
#include "rogue/io/rl_config.h"

#include "kinc/system.h"
#include "kinc/window.h"
#include "kinc/display.h"
#include "kinc/log.h"

void rl_game_loop_foreground_handler(void * user_data)
{
    rl_game_context_t * context = RL_CAST(rl_game_context_t *, user_data);

    context->enable_sleep_in_loop = false;
}

void rl_game_loop_background_handler(void * user_data)
{
    rl_game_context_t * context = RL_CAST(rl_game_context_t * , user_data);

    context->enable_sleep_in_loop = true;
}

void rl_game_loop_shutdown_handler(void * user_data)
{
    rl_game_context_t * context = RL_CAST(rl_game_context_t *, user_data);
    //NOTE(<zshoals> 11-07-2023):> This would be a good place to force a game save (or backup at least)
}

void rl_game_loop_render(rl_game_context_t * context, double dt)
{

}

void rl_game_loop_simulate(rl_game_context_t * context, double dt)
{
    kinc_log(KINC_LOG_LEVEL_INFO, "Simming... %f", dt);

}

void rl_game_loop_update(void * user_data)
{
    rl_game_context_t * context = RL_CAST(rl_game_context_t * , user_data);

    //Logic
    double now = kinc_time();
    context->logic_accumulator += now - context->previous_logic_time;
    context->previous_logic_time = now;

    //NOTE(<zshoals> 11-07-2023):> Maybe janky to update the time here on frames where a logic sim and render both happen?
    context->render_accumulator += now - context->previous_render_time;
    context->previous_render_time = now;

    while (context->logic_accumulator >= context->logic_simulation_rate)
    {
        context->logic_accumulator -= context->logic_simulation_rate;
        rl_game_loop_simulate(context, context->logic_simulation_rate);
    }


    //Render
    memcpy(&(context->render_state), &(context->logic_state), sizeof(rl_game_state_t));

    double render_dt = (kinc_time() - context->previous_render_time) + context->render_accumulator;
    if ( !(context->config.wants_vertical_sync) && context->enable_framerate_limiter)
    {
        //NOTE(<zshoals> 11-07-2023):> Only actually render here if we're not exceeding the framelimit
        //  this code looks redundant, but is not
        if (context->render_accumulator >= (1.0 / RL_CAST(double, context->framerate_limit)))
        {
            rl_game_loop_render(context, render_dt);
            context->render_accumulator = 0.0;
        }
        else
        {
            RL_NO_OPERATION();
        }
    }
    else
    {
        rl_game_loop_render(context, render_dt);
        context->render_accumulator = 0.0;
    }
}

void rl_game_loop_boot(int primary_window)
{
    //NOTE(<zshoals> 11-06-2023):> The entire game context is RIGHT...HERE!~
    //======================================================================
    static rl_game_context_t context = {0};
    //======================================================================

    //Apply various window settings now once Kinc has been initialized
    context.primary_window = primary_window;
    {
        context.config = rl_config_load_from_disk_or_apply_defaults();
        rl_config_save_to_disk(context.config);

        int primary_display_index = kinc_primary_display();
        kinc_display_mode_t mode = kinc_display_current_mode(primary_display_index);

        kinc_window_options_t wo = {0};
        {
            wo.x = -1;
            wo.y = -1;
            wo.width = context.config.window_width;
            wo.height = context.config.window_height;
            wo.display_index = primary_display_index;
            wo.visible = true;
            wo.window_features = 
                KINC_WINDOW_FEATURE_MINIMIZABLE | 
                KINC_WINDOW_FEATURE_MAXIMIZABLE | 
                KINC_WINDOW_FEATURE_RESIZEABLE;

            wo.mode = (context.config.wants_borderless_fullscreen) ? KINC_WINDOW_MODE_FULLSCREEN : KINC_WINDOW_MODE_WINDOW;
        }

        kinc_framebuffer_options_t fbo = {0};
        {
            fbo.frequency = mode.frequency;
            fbo.color_bits = 32;
            fbo.depth_bits = 16;
            fbo.stencil_bits = 8;
            fbo.samples_per_pixel = 1;

            fbo.vertical_sync = context.config.wants_vertical_sync;
        }

        kinc_window_change_framebuffer(context.primary_window, &fbo);
        kinc_window_resize(context.primary_window, wo.width, wo.height);
        int window_half_width = (mode.width / 2) - (context.config.window_width / 2);
        int window_half_height = (mode.height / 2) - (context.config.window_height / 2);
        kinc_window_move(context.primary_window, window_half_width, window_half_height);
        kinc_window_change_mode(context.primary_window, wo.mode);
    }

    //Configure Loop
    {
        context.enable_framerate_limiter = true;
        context.framerate_limit = 300;

        context.logic_simulation_rate = (1.0 / 30.0);
        context.maximum_frametime = (1.0 / 10.0);
        context.maximum_frametime_overruns = 10;
    }


    //NOTE(<zshoals> 11-06-2023):> Begin the main game loop!
    kinc_set_update_callback(&rl_game_loop_update, &context);
    kinc_set_foreground_callback(&rl_game_loop_foreground_handler, &context);
    kinc_set_background_callback(&rl_game_loop_background_handler, &context);
    kinc_set_shutdown_callback(&rl_game_loop_shutdown_handler, &context);

    kinc_start();
}

