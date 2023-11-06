#pragma once

#include "rogue/rl_common.h"

#define RL_CONFIG_BUFFER_CAPACITY 2048
#define RL_CONFIG_MAX_TOKENS 128

#define RL_CONFIG_PATH "heliotrope_fields_game.config"
#define RL_CONFIG_DEFAULT_WINDOW_WIDTH 800
#define RL_CONFIG_DEFAULT_WINDOW_HEIGHT 600
#define RL_CONFIG_DEFAULT_VERTICAL_SYNC true
#define RL_CONFIG_DEFAULT_BORDERLESS_FULLSCREEN false

#define RL_CONFIG_MAX_WINDOW_WIDTH 7680
#define RL_CONFIG_MAX_WINDOW_HEIGHT 4320
#define RL_CONFIG_MIN_WINDOW_WIDTH 800
#define RL_CONFIG_MIN_WINDOW_HEIGHT 600

typedef struct rl_config_t
{
    int window_width;
    int window_height;
    bool wants_vertical_sync; 
    bool wants_borderless_fullscreen; 

} rl_config_t;

bool rl_config_save_to_disk(rl_config_t config);
rl_config_t rl_config_load_from_disk_or_apply_defaults(void);
