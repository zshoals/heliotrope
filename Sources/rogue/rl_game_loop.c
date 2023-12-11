#include "rogue/rl_game_loop.h"

#include "rogue/rl_game_context.h"
#include "rogue/io/rl_config.h"
#include "rogue/rl_window.h"

#include "kinc/system.h"
#include "kinc/threads/thread.h"
#include "kinc/window.h"
#include "kinc/display.h"
#include "kinc/log.h"
#include "kinc/graphics4/graphics.h"

#include <stdio.h>
#include <math.h>

void rl_game_loop_foreground_handler(void * user_data)
{
    rl_game_context_t * context = RL_CAST(rl_game_context_t *, user_data);

    kinc_log(KINC_LOG_LEVEL_INFO, "Window gained focus");
    context->enable_sleep_in_loop = false;
}

void rl_game_loop_background_handler(void * user_data)
{
    rl_game_context_t * context = RL_CAST(rl_game_context_t *, user_data);

    kinc_log(KINC_LOG_LEVEL_INFO, "Window lost focus");
    context->enable_sleep_in_loop = true;
}

void rl_game_loop_shutdown_handler(void * user_data)
{
    rl_game_context_t * context = RL_CAST(rl_game_context_t *, user_data);
    //NOTE(<zshoals> 11-07-2023):> This would be a good place to force a game save (or backup at least)
    kinc_log(KINC_LOG_LEVEL_INFO, "Shutting down...");
}

void rl_game_loop_render(rl_game_context_t * context, double dt)
{
    //TODO(<zshoals> 12-08-2023):> The window flickers on startup if we haven't rendered once before making
    //  the window visible, so we need to actually render once before displaying it to avoid flickering
    kinc_g4_begin(0);
    kinc_g4_clear(KINC_G4_CLEAR_COLOR, 0, 0.0f, 0);
    kinc_g4_end(0);

    kinc_g4_swap_buffers();
}

void rl_game_loop_simulate(rl_game_context_t * context, double dt)
{
    kinc_log(KINC_LOG_LEVEL_INFO, "Simming... %f", dt);
    context->main_window.current.x = kinc_window_x(context->main_window.current._window_id);
    context->main_window.current.y = kinc_window_y(context->main_window.current._window_id);

    rl_window_immediate_add_position(&(context->main_window), (int)(sin(kinc_time()) * 10), (int)(cos(kinc_time()) * 10));


    char title_buffer[256] = {0};
    snprintf(&(title_buffer[0]), 255, "%.20f NERDS", dt);
    rl_window_immediate_set_title(&(context->main_window), &(title_buffer[0]));
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
    if (context->render_accumulator > context->maximum_frametime)
    {
        context->render_accumulator = 0.0;
        context->current_frametime_overruns += 1;
    }
    else
    {
        context->current_frametime_overruns = 0;
    }

    if (context->current_frametime_overruns >= context->maximum_frametime_overruns)
    {
        RL_ASSERT(0);
        kinc_stop();
    }

    memcpy(&(context->render_state), &(context->logic_state), sizeof(rl_game_state_t));

    double render_dt = (kinc_time() - context->previous_render_time) + context->render_accumulator;
    if ( !(context->config.wants_vertical_sync) && context->config.wants_framerate_limiter)
    {
        //NOTE(<zshoals> 11-07-2023):> Only actually render here if we're not exceeding the framelimit
        //  this code looks redundant, but is not
        if (context->render_accumulator >= (1.0 / RL_CAST(double, context->config.framerate_limit)))
        {
            rl_game_loop_render(context, render_dt);
            context->render_accumulator = 0.0;
        }
        else
        {
            RL_NO_OPERATION();
        }
    }
    else //NOTE(<zshoals> 11-07-2023):> We're vsynced; render and vsync will stall the loop itself
    {
        rl_game_loop_render(context, render_dt);
        context->render_accumulator = 0.0;
    }

    if (context->enable_sleep_in_loop)
    {
        kinc_thread_sleep(32);
    }
}

void rl_game_loop_boot(int primary_window)
{
    //NOTE(<zshoals> 11-06-2023):> The entire game context is RIGHT...HERE!~
    //======================================================================
    static rl_game_context_t context = {0};
    //======================================================================

    context.config = rl_config_load_from_disk_or_apply_defaults();
    rl_config_save_to_disk(context.config);

    //Apply various window settings now once Kinc has been initialized
    {
        int primary_display_index = kinc_primary_display();
        rl_window_initialize(&(context.main_window), primary_window, primary_display_index);

        rl_window_pending_begin_changes(&(context.main_window));
        {
            context.main_window.pending_changes.width = context.config.window_width;
            context.main_window.pending_changes.height = context.config.window_height;
            context.main_window.pending_changes.visible = true;
            context.main_window.pending_changes.minimizable = true;
            context.main_window.pending_changes.maximizable = true;
            context.main_window.pending_changes.resizable = true;
            context.main_window.pending_changes.on_top = false;
            context.main_window.pending_changes.borderless_fullscreen = context.config.wants_borderless_fullscreen;
            context.main_window.pending_changes.vertical_sync = context.config.wants_vertical_sync;
        }
        rl_window_pending_apply_changes(&(context.main_window));
        rl_window_immediate_center_window(&(context.main_window));
    }

    //Configure Loop
    {
        //NOTE(<zshoals> 11-07-2023):> Don't allow framerate limit to dip under frametime overruns or bad
        //  things will happen
        //  this should probably be somewhere else, like maybe forced every frame
        if (context.config.framerate_limit < 15) { context.config.framerate_limit = 15; }

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

