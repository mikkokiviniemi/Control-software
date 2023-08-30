#include "text_ui.hpp"
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

constexpr int NUMBER_OF_EMPTY_ROWS{ 15 };

void print_empty_rows(int number_of_rows)
{
    for (int i{ 0 }; i < number_of_rows; ++i)
    {
        std::cout << '\n';
    }
}

// //Display help for commands
// void display_help () {
//     std::cout << "Commands:\n";
//     std::cout 
//     << "soc : speed of conveyor\n"
//     << "display : display values\n"
//     << "hc : heater/cooler toggle\n"
//     << "camera : camera toggle\n"
//     << "apply : apply changes\n";
// }
// //Simple ui for speed of conveyor input return uint8_t
// uint8_t soc_ui (){
//     std::string input;
//     std::cout << "Input for new speed (0-255): ";
//     std::getline(std::cin,input);

//     uint8_t new_speed = static_cast<uint8_t>(std::stoi(input));
//     return new_speed;
// }
// //Simple ui for heater and cooler input return uint8_t
// bool hc_ui (std::string name){
//     std::string input;
//     std::cout << "Input for new " << name << " value (1,0): ";
//     std::getline(std::cin,input);
//     int value = std::stoi(input);
//     return value;
// }
// //Simple ui for camera_toggle input return uint8_t
// bool camera_ui (){
//     std::string input;
//     std::cout << "Input for camera toggle (1,0): ";
//     std::getline(std::cin,input);
//     int camera_toggle = std::stoi(input);
//     return camera_toggle;
// }

void print_sensor_data(const json& sensor_data_json)
{
    std::cout << "conveyor: " << sensor_data_json["speed_of_conveyor"] << '\n';
    std::cout << "heater_1: " << sensor_data_json["heater_1"] << '\n';
    std::cout << "heater_2: " << sensor_data_json["heater_2"] << '\n';
    std::cout << "heater_3: " << sensor_data_json["heater_3"] << '\n';
    std::cout << "cooler: " << sensor_data_json["cooler"] << '\n';
    std::cout << "temperatures: "<< std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][0] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][1] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][2] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][3] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][4] << '\n';
    std::cout << std::fixed << std::setprecision(1) << "\t" << sensor_data_json["temp_sensors"][5] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][6] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][7] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][8] << ' ';
    std::cout << std::fixed << std::setprecision(1) << sensor_data_json["temp_sensors"][9] << '\n';
    std::cout << "qc_camera_fails: " << sensor_data_json["qc_camera_fails"] << '\n';
    std::cout << "timestamp: " << sensor_data_json["time_stamp"] << "\n\n";
}


void print_control_data(const json& control_data_json)
{
    std::cout << "Conveyor target speed: "  << control_data_json["speed_of_conveyor"] << '\n';
    std::cout << "Heater1 on/off: "         << control_data_json["heater_1"] << '\n';
    std::cout << "Heater2 on/off: "         << control_data_json["heater_2"] << '\n';
    std::cout << "Heater3 on/off: "         << control_data_json["heater_3"] << '\n';
    std::cout << "Cooler on/off: "          << control_data_json["cooler"] << '\n';
    std::cout << "Camera on/off: "          << control_data_json["qc_camera_toggle"] << '\n';
    std::cout << "Conveyor manual control: "<< control_data_json["conveyor_manual_control"] << '\n';
    std::cout << "Heater1 manual control: " << control_data_json["heater_1_manual_control"] << '\n';
    std::cout << "Heater2 manual control: " << control_data_json["heater_2_manual_control"] << '\n';
    std::cout << "Heater3 manual control: " << control_data_json["heater_3_manual_control"] << '\n';
    std::cout << "Cooler manual control: "  << control_data_json["cooler_manual_control"] << "\n\n";
}

/*
 ON/OFF TOGGLE: 1 2 3 4 5 6 7
 MANUAL TOGGLE: Q W E R T Y
*/
bool control_data_input(json& output)
{
    std::string choice;
    std::getline(std::cin, choice);

        if (choice == "q") {
            output["heater_1_manual_control"] = !output["heater_1_manual_control"];
        }
        else if (choice == "w") {
            output["heater_2_manual_control"] = !output["heater_2_manual_control"];
        }
        else if (choice == "e") {
            output["heater_3_manual_control"] = !output["heater_3_manual_control"];
        }
        else if (choice == "r") {
            output["cooler_manual_control"] = !output["cooler_manual_control"];
        }
        else if (choice == "t") {
            output["conveyor_manual_control"] = !output["conveyor_manual_control"];
        }
        else if (choice == "y") {
            output["cooler_manual_control"] = !output["cooler_manual_control"];
        }
        else if (choice == "1") {
            output["heater_1"] = !output["heater_1"];
        }
        else if (choice == "2") {
            output["heater_2"] = !output["heater_2"];
        }
        else if (choice == "3") {
            output["heater_3"] = !output["heater_3"];
        }
        else if (choice == "4") {
            output["cooler"] = !output["cooler"];
        }
        else if (choice == "5") {
            output["qc_camera_toggle"] = !output["qc_camera_toggle"];
        }
        else if (choice == "6") {
            
            int speed{ output["speed_of_conveyor"] };
            if (speed < 255)
                ++speed;
            output["speed_of_conveyor"] = speed;
        }
        else if (choice == "7") {
            int speed{ output["speed_of_conveyor"] };
            if (speed > 0)
            --speed;
            output["speed_of_conveyor"] = speed;
        }
        else if (choice == "9") {
            return false;
        }
        else
        {
            return true;
        }
        
    return true;
}

//Main function
int main () {
    json sensor_data_json
    {
        {"speed_of_conveyor", 0 },
        {"heater_1", false},
        {"heater_2", false},
        {"heater_3", false},
        {"cooler", true },
        {"temp_sensors", {25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0}},
        {"qc_camera_fails", 0},
        {"time_stamp", "timestamp"}
    }; 
    
    json control_data_json // input from UI
    {
    {"conveyor_manual_control", true},
    {"cooler", false },
    {"cooler_manual_control", true},
    {"heater_1", false },
    {"heater_1_manual_control", true},
    {"heater_2", false },
    {"heater_2_manual_control", true},
    {"heater_3", false },
    {"heater_3_manual_control", true},
    {"speed_of_conveyor", 0 },
    {"qc_camera_toggle", false }
    };
    
    // input from UI

    MQTT_Client_UI ui_client(ADDRESS_UI, USER_ID_UI);
    ui_client.connect_broker();
    ui_client.subscribe(TOPIC_REC_SENSOR);

    bool ui_is_alive(true);
    
    while (ui_is_alive) {
        // RECEIVE SENSOR_DATA_JSON AND CAMERA_FEED_JSON;
        sensor_data_json = ui_client.sensor_data;
        print_sensor_data(sensor_data_json);
        print_control_data(control_data_json);
        std::cout << "q)Toggle heater1 auto/manual\n";
        std::cout << "w)Toggle heater2 auto/manual\n";
        std::cout << "e)Toggle heater3 auto/manual\n";
        std::cout << "r)Toggle cooler auto/manual\n";
        std::cout << "t)Toggle conveyor auto/manual\n";
        std::cout << "y)Toggle qc-camera auto/manual\n";
        std::cout << "1)Toggle heater 1\n";
        std::cout << "2)Toggle heater 2\n";
        std::cout << "3)Toggle heater 3\n";
        std::cout << "4)Toggle cooler\n";
        std::cout << "5)Toggle qc_camera\n";
        std::cout << "6)Decrease convoyer speed\n";
        std::cout << "7)Increase convoyer speed\n";
        std::cout << "9)Quit\n";
        print_empty_rows(10);

        ui_is_alive = control_data_input(control_data_json);

        // SEND CONTROL_DATA_JSON
        ui_client.publish(TOPIC_SEND_CONTROL, control_data_json.dump());
    }

}
