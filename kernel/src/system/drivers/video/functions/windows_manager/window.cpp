#include "../../driver.h"
#include "window.h"

#include "system/drivers/video/driver.h"
#include "system/gui/vars/colors.h"

void draw_window(window_struct* window) {
    if (!window || !window->visible)
        return;

    uint32_t* bb_ptr = get_backbuffer();
    size_t pitch = get_backbuffer_pitch();

    for (int y = 0; y < window->height; y++) {
        for (int x = 0; x < window->width; x++) {
            bb_ptr[(window->pos_y + y) * pitch + (window->pos_x + x)] = COLOR_WINDOW;
        }
    }
    for (int y = 0; y < (window->height/10); y++) {
        for (int x = 0; x < window->width; x++) {
            bb_ptr[(window->pos_y + y) * pitch + (window->pos_x + x)] = COLOR_TITLEBAR;
        }
    }

    print_at8(window->name, (window->pos_x + 10), (window->pos_y + ((((window->height) / 10) - 8) / 2)), COLOR_WHITE);

    int button_x = window->pos_x + window->width - 55;
    int button_y = window->pos_y + ((window->height / 10) - 8) / 2;

    print_at8("-", button_x, button_y, COLOR_WHITE);
    print_at8("[]", button_x + 16, button_y, COLOR_WHITE);
    print_at8("X", button_x + 42, button_y, COLOR_WHITE);
}

bool is_mouse_over_window(window_struct* window, int mouse_x, int mouse_y)
{
    if (!window || !window->visible)
        return false;

    if (mouse_x >= window->pos_x &&
        mouse_x <= window->pos_x + window->width &&
        mouse_y >= window->pos_y &&
        mouse_y <= window->pos_y + window->height)
    {
        return true;
    }

    return false;
}

bool is_mouse_over_window_title(window_struct* window, int mouse_x, int mouse_y)
{
    if (!window || !window->visible)
        return false;

    int title_height = window->height / 10;

    if (mouse_x >= window->pos_x &&
        mouse_x <= window->pos_x + window->width &&
        mouse_y >= window->pos_y &&
        mouse_y <= window->pos_y + title_height)
    {
        return true;
    }

    return false;
}

window_button get_window_button(window_struct* window, int mouse_x, int mouse_y)
{
    if (!window || !window->visible)
        return BUTTON_NONE;

    int title_height = window->height / 10;

    int button_y = window->pos_y + ((title_height - 8) / 2);

    int minimize_x = window->pos_x + window->width - 55;
    int maximize_x = minimize_x + 16;
    int close_x = minimize_x + 42;


    // sprawdzamy czy mysz jest na wysokości przycisków
    if (mouse_y >= button_y && mouse_y <= button_y + 8)
    {
        // -
        if (mouse_x >= minimize_x &&
            mouse_x <= minimize_x + 8)
        {
            return BUTTON_MINIMIZE;
        }

        // []
        if (mouse_x >= maximize_x &&
            mouse_x <= maximize_x + 16)
        {
            return BUTTON_MAXIMIZE;
        }

        // X
        if (mouse_x >= close_x &&
            mouse_x <= close_x + 8)
        {
            return BUTTON_CLOSE;
        }
    }

    return BUTTON_NONE;
}