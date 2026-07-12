#include "./icons/icons.h"
#include "system/drivers/video/driver.h"

bool last_start_hover = false;

void update_gui_state(int mouse_x, int mouse_y) {
    // Sprawdzamy czy mysz jest nad przyciskiem i aktualizujemy flagę
    if (is_mouse_over_start(mouse_x, mouse_y)) {
        start_hover = true;
    } else {
        start_hover = false;
    }
}