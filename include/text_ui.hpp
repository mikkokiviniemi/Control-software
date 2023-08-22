#ifndef TEXT_UI_HPP
#define TEXT_UI_HPP

#include <iostream>
#include "data_structures.hpp"

#include "../external/json.hpp"

using json = nlohmann::json;

control_data ui_loop(bool &display_ui);

void json_ui(json &output, const json &input, const json &input_2);


#endif

