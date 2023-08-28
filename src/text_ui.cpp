#include "text_ui.hpp"
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

constexpr int NUMBER_OF_EMPTY_ROWS{ 15 };

bool ui_is_alive(true);

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
    std::cout << std::fixed << std::setprecision(1);
    //std::cout << "temperatures: " << sensor_data_json["temp_sensors"] << '\n';
    std::cout << "temperatures: " << sensor_data_json["temp_sensors"][0] << ' ';
    std::cout << sensor_data_json["temp_sensors"][1] << ' ';
    std::cout << sensor_data_json["temp_sensors"][2] << ' ';
    std::cout << sensor_data_json["temp_sensors"][3] << ' ';
    std::cout << sensor_data_json["temp_sensors"][4] << '\n';
    std::cout << "\t" << sensor_data_json["temp_sensors"][5] << ' ';
    std::cout << sensor_data_json["temp_sensors"][6] << ' ';
    std::cout << sensor_data_json["temp_sensors"][7] << ' ';
    std::cout << sensor_data_json["temp_sensors"][8] << ' ';
    std::cout << sensor_data_json["temp_sensors"][9] << '\n';
    std::cout << "qc_camera_status: " << sensor_data_json["qc_camera_status"] << '\n';
    std::cout << "timestamp: " << sensor_data_json["time_stamp"] << "\n\n";
}

void print_camera_feed(const json& camera_feed)
{
    std::cout << "qc_camera_fails: " << camera_feed["qc_camera_fails"] << '\n';
    std::cout << "timestamp: " << camera_feed["time_stamp"] << "\n\n";
}
/*
 ON/OFF TOGGLE: 1 2 3 4 5 6 7
 MANUAL TOGGLE: Q W E R T Y
*/
void control_data_input(json& output)
{
    while (ui_is_alive)
    {

        std::string choice;
        std::getline(std::cin, choice);

        std::scoped_lock scp_lck(mtx);

        if (choice == "q") {
            output["heater1_manual_control"] = !output["heater1_manual_control"];
        }
        else if (choice == "w") {
            output["heater2_manual_control"] = !output["heater2_manual_control"];
        }
        else if (choice == "e") {
            output["heater3_manual_control"] = !output["heater3_manual_control"];
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
            output["qc_camera_status"] = !output["qc_camera_status"];
        }
        else if (choice == "6") {
            uint8_t speed{ output["speed_of_conveyor"] };
            output["speed_of_conveyor"] = ++speed;
        }
        else if (choice == "7") {
            uint8_t speed{ output["speed_of_conveyor"] };
            output["speed_of_conveyor"] = --speed;
        }
        else if (choice == "9") {
            ui_is_alive = false;
            return;
        }

    }
}

// //Simple text ui 
// void json_ui (json& output, const json& input, const json& input_2) {
    
//     while(true) {
//         std::cout << "[help,manual,soc,hc,camera,display,apply] ";
//         std::string choice;
//         std::string manual;
//         std::cout << ">";
//         std::getline(std::cin,choice);

//         if (choice == "help") {
//             display_help();
//         }
//         else if (choice == "manual") {
//             std::cout << ">";
//             std::getline(std::cin,manual);
//             if (manual == "true") {
//                 output["heater1_manual_control"] = true;
//                 output["heater2_manual_control"] = true;
//                 output["heater3_manual_control"] = true;
//                 output["cooler_manual_control"] = true;
//                 output["conveyor_manual_control"] = true;
//             } else if (manual == "false") {
//                 output["heater1_manual_control"] = false;
//                 output["heater2_manual_control"] = false;
//                 output["heater3_manual_control"] = false;
//                 output["cooler_manual_control"] = false;
//                 output["conveyor_manual_control"] = false;
//             }
//         }
//         else if (choice == "soc") {
//             output["speed_of_conveyor"] = soc_ui();
//         }
//         else if (choice == "hc") {
//             output["heater_1"] = hc_ui("heater_1");
//             output["heater_2"] = hc_ui("heater_2");
//             output["heater_3"] = hc_ui("heater_3");
//             output["cooler"] = hc_ui("cooler");
//         }
//         else if (choice == "camera") {
//             output["qc_camera_status"] = camera_ui();
//         }
//         else if (choice == "display") {
//             std::cout << input.dump(4) << "\n";
//             std::cout << input_2.dump(4) << "\n";
//             std::cout << output.dump(4) << "\n";
//         }
//         else if (choice == "apply") {
//             std::cout << "Changes applied!\n";
//             break;
//         }
//         else {
//             std::cout << "Invalid command!\n";
//         }
//     }
// }


//Main function
int main () {
    json sensor_data_json
    {
        {"speed_of_conveyor", 0 },
        {"heater_1", false},
        {"heater_2", false},
        {"heater_3", false},
        {"cooler", true },
        {"qc_camera_status", false},
        {"temp_sensors", {25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0}},
        {"time_stamp", "timestamp"}
    }; 
    print_sensor_data(sensor_data_json);
    
    // output to UI
    json camera_feed_json
    {
        {"qc_camera_fails", 0},
        {"time_stamp", "timestamp"}
    };    // output to UI
    print_camera_feed(camera_feed_json);
    json control_data_json // input from UI
    {
    {"speed_of_conveyor", 0 },
    {"heater_1", false },
    {"heater_2", false },
    {"heater_3", false },
    {"cooler", false },
    {"qc_camera_status", false },
    {"conveyor_manual_control", true},
    {"heater1_manual_control", true},
    {"heater2_manual_control", true},
    {"heater3_manual_control", true},
    {"cooler_manual_control", true}
    };
    
     // input from UI

    MQTT_Client_UI ui_client(ADDRESS_UI, USER_ID_UI);
    ui_client.connect_broker();
    ui_client.subscribe(TOPIC_REC_SENSOR);
    ui_client.subscribe(TOPIC_REC_CAMERA);

    while (ui_is_alive) {
        // RECEIVE SENSOR_DATA_JSON AND CAMERA_FEED_JSON;
        sensor_data_json = ui_client.sensor_data;
        camera_feed_json = ui_client.camera_data;
        print_sensor_data(sensor_data_json);
        print_camera_feed(camera_feed_json);
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

        std::jthread input_thread(control_data_input, std::ref(control_data_json));

        

        std::scoped_lock scp_lck(mtx);
        // SEND CONTROL_DATA_JSON
        ui_client.publish(TOPIC_SEND_CONTROL, control_data_json.dump());
    }

}
