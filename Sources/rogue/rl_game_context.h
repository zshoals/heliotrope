#pragma once

#include "rogue/rl_common.h"

#include "rogue/io/rl_config.h"
#include "rogue/rl_game_state.h"

#include "kinc/window.h"

typedef struct rl_game_context_t
{
    int primary_window;
    rl_config_t config;

    bool enable_sleep_in_loop;

    double previous_logic_time;
    double logic_simulation_rate;
    double logic_accumulator;

    double previous_render_time;
    double render_accumulator;

    bool enable_framerate_limiter;
    int32_t framerate_limit;

    double maximum_frametime;
    int32_t current_frametime_overruns;
    int32_t maximum_frametime_overruns;
    
    rl_game_state_t logic_state;
    rl_game_state_t render_state;

} rl_game_context_t;

