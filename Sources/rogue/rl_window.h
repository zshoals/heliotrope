#pragma once

#include <stdbool.h>
#include "kinc/window.h"
#include "kinc/display.h"

typedef struct rl_window_state_t
{
    int x;
    int y;
    int width;
    int height;
    int display_index;
    int window_id;

    bool visible;
    bool minimizable;
    bool maximizable;
    bool resizable;
    bool on_top;

    bool borderless_fullscreen;
    bool vertical_sync;

    kinc_display_mode_t mode;
} rl_window_state_t;

typedef struct rl_window_t
{
    bool initialized;
    bool change_in_progress;

    rl_window_state_t current;
    rl_window_state_t pending_changes;
} rl_window_t;

void rl_window_initialize(rl_window_t * win, int window_id, int display_index);

void rl_window_immediate_set_title(rl_window_t * win, char const * title);

void rl_window_immediate_set_position(rl_window_t * win, int x, int y);
void rl_window_immediate_add_position(rl_window_t * win, int x, int y);
void rl_window_immediate_set_window_size(rl_window_t * win, int width, int height);
void rl_window_immediate_set_visibility(rl_window_t * win, bool visible);
void rl_window_immediate_set_minimizable(rl_window_t * win, bool minimizable);
void rl_window_immediate_set_maximizable(rl_window_t * win, bool maximizable);
void rl_window_immediate_set_resizable(rl_window_t * win, bool resizable);
void rl_window_immediate_set_on_top(rl_window_t * win, bool on_top);
void rl_window_immediate_set_borderless_fullscreen(rl_window_t * win, bool fullscreen);
void rl_window_immediate_set_vertical_sync(rl_window_t * win, bool vsync);

void rl_window_pending_begin_changes(rl_window_t * win);
void rl_window_pending_apply_changes(rl_window_t * win);
void rl_window_pending_abort_changes(rl_window_t * win);