#include "text_ui.hpp"

#include <iostream>

//Simple ui for speed of conveyor input return uint8_t
uint8_t speed_of_conveyor_ui (){
    std::string input;
    std::cout << "Iput for new speed: ";
    std::getline(std::cin,input);

    uint8_t new_speed = std::stoi(input);
    return new_speed;
}
//Simple ui for heater and cooler input return uint8_t
uint8_t heaters_cooler_ui (){
    std::string input;
    std::cout << "Iput for new h/c value: ";
    std::getline(std::cin,input);
    uint8_t value = std::stoi(input);
    return value;
}
//Simple ui for camera_toggle input return uint8_t
uint8_t camera_toggle_ui (){
    std::string input;
    std::cout << "Iput for camera toggle: ";
    std::getline(std::cin,input);

    uint8_t camera_toggle = std::stoi(input);
    return camera_toggle;
}
//Display help for commands
void display_help () {
    std::cout << "Commands:\n";
    std::cout 
    << "help : display help\n"
    << "soc : speed of conveyor\n"
    << "display : display values\n"
    << "h/c : heater/cooler toggle\n"
    << "camera : camera toggle\n"
    << "quit : Quit UI\n";
}
//Print speed_of_conveyor, heater_cooler, camera_toggle
void display_values (uint8_t& speed_of_conveyor, uint8_t& heaters_cooler, uint8_t& camera_toggle) {
    std::cout 
    << "Speed_of_conveyor: " << static_cast<int>(speed_of_conveyor) << "\n"
    << "heaters_cooler: " << static_cast<int>(heaters_cooler) << "\n"
    << "camera_toggle: " << static_cast<int>(camera_toggle) << "\n";
}


// Main ui loop
void ui_loop (bool& display_ui, uint8_t& speed_of_conveyor, uint8_t& heaters_cooler, uint8_t& camera_toggle) {
    display_help();
    while(display_ui) {
        std::string choice;
        std::cout << ">";
        std::getline(std::cin,choice);

        if (choice == "help") {
            display_help();
        }
        else if (choice == "soc") {
            speed_of_conveyor = speed_of_conveyor_ui();
        }
        else if (choice == "h/c") {
            heaters_cooler = heaters_cooler_ui();
        }
        else if (choice == "camera") {
            camera_toggle = camera_toggle_ui();
        }
        else if (choice == "display") {
            display_values(speed_of_conveyor,heaters_cooler,camera_toggle);
        }
        else if (choice == "quit") {
            std::cout << "quit\n";
            display_ui = false;
        }
        else {
            std::cout << "Invalid command!\n";
        }
    }

}