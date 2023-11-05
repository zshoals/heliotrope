#pragma once

#include "rogue/rl_common.h"

#define RL_CONFIG_BUFFER_CAPACITY 1024
#define RL_CONFIG_PATH "config.txt"

typedef struct rl_config_t
{
    int window_width;
    int window_height;
    bool wants_vertical_sync; 
    bool wants_borderless_fullscreen; 

} rl_config_t;

bool rl_config_save_to_disk(rl_config_t config);
rl_config_t rl_config_load_from_disk_or_apply_defaults(char const * path);
