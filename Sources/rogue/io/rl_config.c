#include "rogue/io/rl_config.h"

#include "kinc/system.h"
#include <stdio.h>
#include <ctype.h>

bool rl_config_save_to_disk(rl_config_t config)
{
    char buffer[RL_CONFIG_BUFFER_CAPACITY] = {0};

    char const * format = "window_width:%d\n"
    "window_height:%d\n"
    "wants_vertical_sync:%d\n"
    "wants_borderless_fullscreen:%d\n";

    int written = snprintf(
        &(buffer[0]), 
        RL_CONFIG_BUFFER_CAPACITY, 
        format, 
        config.window_width, 
        config.window_height, 
        RL_CAST(int, config.wants_vertical_sync),
        RL_CAST(int, config.wants_borderless_fullscreen)
    );

    if ( (written < RL_CONFIG_BUFFER_CAPACITY) && (written > 0) )
    {
        kinc_save_save_file(RL_CONFIG_PATH, &(buffer[0]), written);
        return true;
    }
    else
    {
        return false;
    }
}

rl_config_t rl_config_load_from_disk_or_apply_defaults(char const * path)
{
    //Note(zshoals): Always set defaults, just in case of missing parameters in the config file
    rl_tryload_config_t config = {0};
    {
        config.window_width = 800;
        config.window_height = 600;
        config.wants_vertical_sync = true;
        config.wants_borderless_fullscreen = false;
    }

    //TODO(zshoals): kinc_save_file_loaded --only ever returns true!!!-- this needs to be patched in kinc...
    //  check file save size instead which gets updated properly
    kinc_load_save_file(RL_CONFIG_PATH);

    size_t file_size = kinc_get_save_file_size();
    if (file_size > 0)
    {
        char const * file = RL_CAST(char const *, kinc_get_save_file());
        //TODO(zshoals): Actually parse it....we need that generic parser again :/
    }
    return config;
}
