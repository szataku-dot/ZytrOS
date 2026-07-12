#include "./icons/icons.h"
#include "system/drivers/video/driver.h"
#include "system/drivers/memory/driver.h"
#include "system/drivers/keyboard/driver.h"
#include "system/drivers/mouse/driver.h"
#include "./vars/colors.h"

extern limine_framebuffer* fb;

bool last_start_hover = false;

const uint32_t menu_w = 300;
const uint32_t menu_h = 400;
uint32_t menu_x = 0;
uint32_t menu_y = 0;

static uint32_t start_menu_background[menu_h * menu_w];

void update_gui_state(int mouse_x, int mouse_y) {
    // Sprawdzamy czy mysz jest nad przyciskiem i aktualizujemy flagę
    if (is_mouse_over_start(mouse_x, mouse_y)) {
        start_hover = true;
    } else {
        start_hover = false;
    }
}

void save_background(int x, int y, int width, int height)
{
    uint32_t* backbuffer = get_backbuffer();
    size_t pitch = get_backbuffer_pitch();

    for (int row = 0; row < height; row++)
    {
        memcpy(
            &start_menu_background[row * width],
            &backbuffer[(y + row) * pitch + x],
            width * sizeof(uint32_t)
        );
    }
}

void restore_background(int x, int y, int width, int height)
{
    uint32_t* backbuffer = get_backbuffer();
    size_t pitch = get_backbuffer_pitch();

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            uint32_t pixel = start_menu_background[row * width + col];

            if (pixel == COLOR_NASUA_START_MENU ||
                pixel == COLOR_NASUA_START_MENU_P)
            {
                pixel = COLOR_NASUA_BG;
            }

            backbuffer[(y + row) * pitch + (x + col)] = pixel;
        }
    }
}

void draw_start_menu(int x1, int y1, int x2, int y2) {
    int panel_width = 45;

    draw_rect(x1 + (menu_w - panel_width), y1, x2 - (menu_w - panel_width), y2, COLOR_NASUA_START_MENU);
    draw_rect(x1, y1, x2 - (menu_w - panel_width), y2, COLOR_NASUA_START_MENU_P);

    draw_start_menu_system_icons((x1 + (panel_width - 32) / 2), (y1 + menu_h - (32 * 2) - 16));
}

void open_start_menu() {
    menu_y = fb->height - 36 - menu_h;
    save_background(menu_x, menu_y, menu_w, menu_h);
    draw_start_menu(menu_x, menu_y, menu_w, menu_h + menu_y);
    restore_mouse_backdrop();
    save_mouse_backdrop();
    draw_mouse_cursor();
    is_menu_start_open = true;
    shell_input_enabled = false;
}

void close_start_menu() {
    menu_y = fb->height - 36 - menu_h;
    restore_background(menu_x, menu_y, menu_w, menu_h);
    is_menu_start_open = false;
    shell_input_enabled = true;
}