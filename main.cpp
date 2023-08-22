#include "text_ui.hpp"

#include <iostream>
#include <vector>
#include <chrono>
#include <bitset>
#include "external/json.hpp"

struct sensor_data {
    uint8_t speed_of_conveyor;
    uint16_t qc_camera_fails;

    int16_t temp_sensor01;
    int16_t temp_sensor02;
    int16_t temp_sensor03;
    int16_t temp_sensor04;
    int16_t temp_sensor05;
    int16_t temp_sensor06;
    int16_t temp_sensor07;
    int16_t temp_sensor08;
    int16_t temp_sensor09;
    int16_t temp_sensor10;

    std::time_t time_stamp;
};
/*
struct control_data {
    uint8_t speed_of_conveyor;
    uint8_t heaters_cooler;
    uint8_t camera_toggle;
};*/

//Simple data_generator
std::vector<sensor_data> dummy_data_generator(int amount) {
    std::vector<sensor_data> dummy_data;
    for (int i = 0; i < amount; i++) {
        sensor_data data_block = {1,1,1,1,1,1,1,1,1,1,1,1,std::time(nullptr)};
        dummy_data.push_back(data_block);
    }
    return dummy_data;
}

//Convert Json to control_data
control_data json_to_control_data(const nlohmann::json& json_elem) {
    control_data new_data;
    new_data.speed_of_conveyor = json_elem["speed_of_conveyor"];
    bool heater_1 = json_elem["heater_1"];
    bool heater_2 = json_elem["heater_2"];
    bool heater_3 = json_elem["heater_3"];
    bool cooler = json_elem["cooler"];
    new_data.heaters_cooler = (heater_1 << 0) | (heater_2 << 1) | (heater_3 << 2) | (cooler << 3);
    new_data.camera_toggle = json_elem["qc_camera_status"];
    return new_data;
}
//Function to display bits
void display_bits(uint8_t value) {
    std::bitset<8> bits(value);
    std::cout << bits << "\n";
}

/*
JSON Output:
{
    "speed_of_conveyor" : int,
    "heater_1": bool,
    "heater_2": bool,
    "heater_3": bool,
    "cooler": bool,
    "qc_camera_status": bool,
    "temp_sensor01": float,
    "temp_sensor02": float,
    "temp_sensor03": float,
    "temp_sensor04": float,
    "temp_sensor05": float,
    "temp_sensor06": float,
    "temp_sensor07": float,
    "temp_sensor08": float,
    "temp_sensor09": float,
    "temp_sensor10": float,
    "time_stamp": value
}
*/
/*
JSON Output_2:

{
    "qc_camera_fails": ???,
    "time_stamp": value
}
*/

/*
JSON Commands:
{
    "speed_of_conveyor" : int,
    "heater_1": bool,
    "heater_2": bool,
    "heater_3": bool,
    "cooler": bool,
    "qc_camera_status": bool,
}
*/

int main()
{
    bool systems_online{ true };
    uint8_t speed_of_conveyor = 0x00;
    uint8_t heaters_cooler = 0x00;
    uint8_t camera_toggle = 0x00;

    /*bool manual_heaters{ false };
    bool manual_cooler{ false };
    bool manual_conveyor{ false };
    bool qc_camera{ false };
    */
    

    while (systems_online)
    {
        //read_sensor_data()
        
    //if UI decides to manual override, it will circumvent automatic control
        //read_ui_control()


    // The control functions writes the control data directly to shared memory
        //conveyor_control()
        //heating_control()
        //cooling_control()
        
        //data_transformation()
        //send_data_to_ui()
        //if (logging_enabled())
        {
            //log_data()    
        }

        // Test JSON data
        std::string json_string = R"(
            {
                "speed_of_conveyor": 5,
                "heater_1": true,
                "heater_2": true,
                "heater_3": true,
                "cooler": false,
                "qc_camera_status": true
            }
        )";

        nlohmann::json json_data = nlohmann::json::parse(json_string);
        control_data data = json_to_control_data(json_data);

        std::cout << "Speed of Conveyor: ";
        std::cout << int(data.speed_of_conveyor) << "\n";
        std::cout << "Heaters and Cooler: ";
        display_bits(data.heaters_cooler);
        std::cout << "Camera Toggle: ";
        display_bits(data.camera_toggle);


        data = ui_loop(systems_online);

    }

    return 0;
}
