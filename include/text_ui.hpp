#ifndef TEXT_UI_HPP
#define TEXT_UI_HPP

#include <iostream>
#include "data_structures.hpp"
#include "json.hpp"
#include "mqtt_client_UI.hpp"

using json = nlohmann::json;

void print_control_data(const json& control_data_json);
void print_sensor_data(const json& sensor_data_json);

int main ();

#endif

