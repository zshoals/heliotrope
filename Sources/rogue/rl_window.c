#include "rogue/rl_window.h"

#include "rogue/rl_common.h"

#include "kinc/window.h"
#include "kinc/display.h"

void rl_window_immediate_set_title(rl_window_t * win, char const * title)
{
    kinc_window_set_title(win->current._window_id, title);
}
void rl_window_immediate_set_position(rl_window_t * win, int x, int y)
{
    win->current.x = x;
    win->current.y = y;

    kinc_window_move(win->current._window_id, x, y);
}

void rl_window_immediate_add_position(rl_window_t * win, int x, int y)
{
    win->current.x += x;
    win->current.y += y;

    kinc_window_move(win->current._window_id, win->current.x, win->current.y);
}

void rl_window_immediate_center_window(rl_window_t * win)
{
    win->current.x = (win->current._mode.width / 2) - (win->current.width / 2);
    win->current.y = (win->current._mode.height / 2) - (win->current.height / 2);

    kinc_window_move(win->current._window_id, win->current.x, win->current.y);
}

void rl_window_immediate_set_window_size(rl_window_t * win, int width, int height)
{
    win->current.width = width;
    win->current.height = height;

    kinc_window_resize(win->current._window_id, width, height);
}

void rl_window_immediate_add_window_size(rl_window_t * win, int width, int height)
{
    win->current.width += width;
    win->current.height += height;

    kinc_window_resize(win->current._window_id, width, height);
}

void rl_window_immediate_set_visibility(rl_window_t * win, bool visible)
{
    if (win->current.visible == visible)
    {
        return;
    }

    win->current.visible = visible;

    if (visible)
    {
        kinc_window_show(win->current._window_id);
    }
    else
    {
        kinc_window_hide(win->current._window_id);
    }
}

static inline void rl_window_internal_recalc_and_reapply_current_window_features(rl_window_t * win)
{
    int features =
        ((win->current.minimizable) ? KINC_WINDOW_FEATURE_MINIMIZABLE : 0) |
        ((win->current.maximizable) ? KINC_WINDOW_FEATURE_MAXIMIZABLE : 0) |
        ((win->current.resizable) ? KINC_WINDOW_FEATURE_RESIZEABLE : 0) |
        ((win->current.on_top) ? KINC_WINDOW_FEATURE_ON_TOP : 0);

        kinc_window_change_features(win->current._window_id, features);
}

void rl_window_immediate_set_minimizable(rl_window_t * win, bool minimizable)
{
    win->current.minimizable = minimizable;
    rl_window_internal_recalc_and_reapply_current_window_features(win);
}

void rl_window_immediate_set_maximizable(rl_window_t * win, bool maximizable)
{
    win->current.maximizable = maximizable;
    rl_window_internal_recalc_and_reapply_current_window_features(win);
}

void rl_window_immediate_set_resizable(rl_window_t * win, bool resizable)
{
    win->current.resizable = resizable;
    rl_window_internal_recalc_and_reapply_current_window_features(win);
}

void rl_window_immediate_set_on_top(rl_window_t * win, bool on_top)
{
    win->current.on_top = on_top;
    rl_window_internal_recalc_and_reapply_current_window_features(win);
}

void rl_window_immediate_set_borderless_fullscreen(rl_window_t * win, bool fullscreen)
{
    if (win->current.borderless_fullscreen == fullscreen)
    {
        return;
    }

    win->current.borderless_fullscreen = fullscreen;

    if (fullscreen)
    {
        kinc_window_change_mode(win->current._window_id, KINC_WINDOW_MODE_FULLSCREEN);
    }
    else
    {
        kinc_window_change_mode(win->current._window_id, KINC_WINDOW_MODE_WINDOW);
    }

    //NOTE(<zshoals> 12-11-2023):> kinc_window_change_mode causes the window to be shown, so we need to rehide the window on mode change, lol
    if (!win->current.visible)
    {
        kinc_window_hide(win->current._window_id);
    }
}

void rl_window_immediate_set_vertical_sync(rl_window_t * win, bool vsync)
{
    if (win->current.vertical_sync == vsync)
    {
        return;
    }

    win->current.vertical_sync = vsync;

    kinc_framebuffer_options_t fbo = {0};
    {
        fbo.frequency = win->current._mode.frequency;
        fbo.color_bits = 32;
        fbo.depth_bits = 16;
        fbo.stencil_bits = 8;
        fbo.samples_per_pixel = 1;

        fbo.vertical_sync = win->current.vertical_sync;
    }

    kinc_window_change_framebuffer(win->current._window_id, &fbo);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void rl_window_pending_begin_changes(rl_window_t * win)
{
    RL_ASSERT(!win->_change_in_progress);

    win->_change_in_progress = true;
}

void rl_window_pending_abort_changes(rl_window_t * win)
{
    RL_ASSERT(win->_change_in_progress);

    win->_change_in_progress = false;
    win->pending_changes = win->current;
}

void rl_window_pending_apply_changes(rl_window_t * win)
{
    RL_ASSERT(win->_initialized);
    RL_ASSERT(win->_change_in_progress);

    kinc_framebuffer_options_t fbo = {0};
    {
        fbo.frequency = win->pending_changes._mode.frequency;
        fbo.color_bits = 32;
        fbo.depth_bits = 16;
        fbo.stencil_bits = 8;
        fbo.samples_per_pixel = 1;

        fbo.vertical_sync = win->pending_changes.vertical_sync;
    }

    kinc_window_change_framebuffer(win->pending_changes._window_id, &fbo);
    kinc_window_resize(win->pending_changes._window_id, win->pending_changes.width, win->pending_changes.height);
    rl_window_internal_recalc_and_reapply_current_window_features(win);
    kinc_window_move(win->pending_changes._window_id, win->pending_changes.x, win->pending_changes.y);
    kinc_window_change_mode(win->pending_changes._window_id, (win->pending_changes.borderless_fullscreen) ? KINC_WINDOW_MODE_FULLSCREEN : KINC_WINDOW_MODE_WINDOW);

    //NOTE(<zshoals> 12-11-2023):> kinc_window_change_mode causes the window to be shown, so we need to rehide the window on mode change, lol
    if (!win->pending_changes.visible)
    {
        kinc_window_hide(win->pending_changes._window_id);
    }

    win->current = win->pending_changes;
    win->_change_in_progress = false;
}


void rl_window_initialize(rl_window_t * win, int window_id, int display_index)
{
    win->pending_changes.x = -1;
    win->pending_changes.y = -1;
    win->pending_changes.width = 800;
    win->pending_changes.height = 600;
    win->pending_changes._window_id = window_id;
    win->pending_changes._display_index = display_index;

    win->pending_changes.visible = false;
    win->pending_changes.minimizable = true;
    win->pending_changes.maximizable = true;
    win->pending_changes.resizable = true;
    win->pending_changes.on_top = false;

    win->pending_changes.borderless_fullscreen = false;
    win->pending_changes.vertical_sync = true;

    kinc_display_mode_t mode = kinc_display_current_mode(display_index);
    win->pending_changes._mode = mode;

    kinc_framebuffer_options_t fbo = {0};
    {
        fbo.frequency = win->pending_changes._mode.frequency;
        fbo.color_bits = 32;
        fbo.depth_bits = 16;
        fbo.stencil_bits = 8;
        fbo.samples_per_pixel = 1;

        fbo.vertical_sync = win->pending_changes.vertical_sync;
    }

    kinc_window_options_t wo = {0};
    {
        wo.x = win->pending_changes.x;
        wo.y = win->pending_changes.y;
        wo.width = win->pending_changes.width;
        wo.height = win->pending_changes.height;
        wo.display_index = win->pending_changes._display_index;
        wo.visible = win->pending_changes.visible;
        wo.window_features = 
            ((win->pending_changes.minimizable) ? KINC_WINDOW_FEATURE_MINIMIZABLE : 0) | 
            ((win->pending_changes.maximizable) ? KINC_WINDOW_FEATURE_MAXIMIZABLE : 0) | 
            ((win->pending_changes.resizable) ? KINC_WINDOW_FEATURE_RESIZEABLE : 0) | 
            ((win->pending_changes.on_top) ? KINC_WINDOW_FEATURE_ON_TOP : 0);

        wo.mode = (win->pending_changes.borderless_fullscreen) ? KINC_WINDOW_MODE_FULLSCREEN : KINC_WINDOW_MODE_WINDOW;
    }

    win->current = win->pending_changes;
    win->_initialized = true;
    win->_change_in_progress = true;

    rl_window_pending_apply_changes(win);
}
