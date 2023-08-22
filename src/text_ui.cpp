#include "text_ui.hpp"
#include <iostream>

//Display help for commands
void display_help () {
    std::cout << "Commands:\n";
    std::cout 
    << "soc : speed of conveyor\n"
    << "display : display values\n"
    << "hc : heater/cooler toggle\n"
    << "camera : camera toggle\n"
    << "quit : Quit UI\n";
}
//Simple ui for speed of conveyor input return uint8_t
int soc_ui (){
    std::string input;
    std::cout << "Iput for new speed: ";
    std::getline(std::cin,input);

    int new_speed = std::stoi(input);
    return new_speed;
}
//Simple ui for heater and cooler input return uint8_t
bool hc_ui (std::string name){
    std::string input;
    std::cout << "Iput for new " << name << " value (1,0): ";
    std::getline(std::cin,input);
    int value = std::stoi(input);
    return value;
}
//Simple ui for camera_toggle input return uint8_t
bool camera_ui (){
    std::string input;
    std::cout << "Iput for camera toggle: ";
    std::getline(std::cin,input);
    int camera_toggle = std::stoi(input);
    return camera_toggle;
}
//Simple text ui 
void json_ui (json& output,const json& input, const json& input_2) {
    output["speed_of_conveyor"] = input["speed_of_conveyor"];
    output["heater_1"] = input["heater_1"];
    output["heater_2"] = input["heater_2"];
    output["heater_3"] = input["heater_3"];
    output["cooler"] = input["cooler"];
    output["qc_camera_status"] = input["qc_camera_status"];

    while(true) {
        std::cout << "[help,soc,hc,camera,display,quit] ";
        std::string choice;
        std::cout << ">";
        std::getline(std::cin,choice);

        if (choice == "help") {
            display_help();
        }  
        else if (choice == "soc") {
            output["speed_of_conveyor"] = soc_ui();
        }
        else if (choice == "hc") {
            output["heater_1"] = hc_ui("heater_1");
            output["heater_2"] = hc_ui("heater_2");
            output["heater_3"] = hc_ui("heater_3");
            output["cooler"] = hc_ui("cooler");
        }
        else if (choice == "camera") {
            output["qc_camera_status"] = camera_ui();
        }
        else if (choice == "display") {
            std::cout << input.dump(4) << "\n";
            std::cout << input_2.dump(4) << "\n";
            std::cout << output.dump(4) << "\n";
        }
        else if (choice == "quit") {
            std::cout << "quit\n";
            break;
        }
        else {
            std::cout << "Invalid command!\n";
        }
    }
}