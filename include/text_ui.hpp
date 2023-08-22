#ifndef TEXT_UI_HPP
#define TEXT_UI_HPP

#include <stdint.h>

//Control data structure
struct control_data {
    uint8_t speed_of_conveyor;
    uint8_t heaters_cooler;
    uint8_t camera_toggle;
};

control_data ui_loop(bool &display_ui);

#endif

