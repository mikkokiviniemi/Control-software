#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "data_structures.hpp"
#include "input_validation.hpp"
#include "automatic_controls.hpp"
#include "json_output.hpp"
#include "failure_detection.hpp"

#include "mqtt_client.hpp"
#include "mqtt/async_client.h"

#include <cstdint>

TEST_CASE("Data validation")
{
    sensor_data sensor_input{ 0, 0, 1250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    control_data ctrl_data{150, 0b1111, 2, 2 };

    static sensor_data sensor_input_empty;
    static control_data ctrl_data_empty;

    uint16_t failed_sensor_input_validation{ temperature_input_validation(sensor_input) };
    uint8_t failed_control_input_validation{ control_input_validation(ctrl_data) };

    std::cout << failed_temp_sensors(sensor_input);
    std::cout << invalid_control_data(ctrl_data);

    CHECK((failed_sensor_input_validation & FAILED_TEMP_SENSOR01));
    CHECK(failed_control_input_validation == 0b00000111);

    CHECK(sensor_data_is_empty(sensor_input_empty));
    CHECK(control_data_is_empty(ctrl_data_empty));

    CHECK(heater_on(ctrl_data, 0));
    CHECK(heater_on(ctrl_data, 1));
    CHECK(heater_on(ctrl_data, 2));
    
    CHECK(!heater_on(ctrl_data_empty, 0));
    CHECK(!heater_on(ctrl_data_empty, 1));
    CHECK(!heater_on(ctrl_data_empty, 2));

    ctrl_data.camera_toggle = 1;
    ctrl_data.cooler = 0;
    ctrl_data.heaters = 0b111;
    ctrl_data.speed_of_conveyor = 150;

    sensor_input.speed_of_conveyor = 150;
    sensor_input.temp_sensor01 = 250;

    std::string failures{ failure_detection(ctrl_data, sensor_input) };
    for (int i{ 0 }; i < 5; ++i)
    {
        sensor_input.temp_sensor01 -= 10;
        sensor_input.temp_sensor02 -= 10;
        sensor_input.temp_sensor03 -= 10;
        sensor_input.temp_sensor04 -= 10;
        sensor_input.temp_sensor05 -= 10;
        failures = failure_detection(ctrl_data, sensor_input);

    }
    
    std::cout << failures;
    CHECK(failures == std::string{ "heaters and/or cooler possibly faulty " });

    sensor_input.temp_sensor01 += 50;
    sensor_input.temp_sensor02 += 50;
    sensor_input.temp_sensor03 += 50;
    sensor_input.temp_sensor04 += 50;
    sensor_input.temp_sensor05 += 50;

    for (int i{ 0 }; i < 5; ++i)
    {
        failures = failure_detection(ctrl_data, sensor_input);
    }
    
    CHECK(failures == std::string{ "" });
    ctrl_data.speed_of_conveyor = 200;
    
    for (int i{ 0 }; i < 20; ++i)
    {
        failures = failure_detection(ctrl_data, sensor_input);
    }
    std::cout << failures << '\n';
    CHECK(failures == std::string{ "conveyor possibly faulty " });

}

TEST_CASE("Json")
{
    uint8_t raw_speed{ 220 }; //518
    int ui_speed{ 550 }; //234

    CHECK(conveyor_speed_raw_to_ui(raw_speed) == 518);
    CHECK(conveyor_speed_ui_to_raw(ui_speed) == 234);

    
}

TEST_CASE("MQTT Client")
{
    json control_data_json // input from UI
    {
        {"speed_of_conveyor", 0 },
        {"heater_1", false },
        {"heater_2", false },
        {"heater_3", false },
        {"cooler", false },
        {"qc_camera_toggle", false },
        {"conveyor_manual_control", true},
        {"heater_1_manual_control", true},
        {"heater_2_manual_control" , true},
        {"heater_3_manual_control", true},
        {"cooler_manual_control", true}
    };

    const std::string ADDRESS = "tcp://test.mosquitto.org:1883";
    const std::string TEST_ID_1 = "control_sw_1";
    const std::string TEST_ID_2 = "user_sw";
    const std::string TOPIC_SEND_SENSOR = "test_sw_topic";

    // Create two clients and connect to test server
    MQTT_Client test_client_sender (ADDRESS, TEST_ID_1);
    MQTT_Client test_client_user (ADDRESS, TEST_ID_2);
    test_client_sender.connect_broker();
    test_client_user.connect_broker();

    // Subscribe to topic with user, send to topic with sender 
    test_client_user.subscribe(TOPIC_SEND_SENSOR);
    test_client_sender.publish(TOPIC_SEND_SENSOR, control_data_json.dump());
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // wait 0,5 sec

    // Check to see that data received is the same as published by other client
    CHECK(test_client_user.input_control_data == json::parse(control_data_json.dump()));
}



TEST_CASE("Automatic control")
{
    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    control_data ctrl_data;
    json control_data_json
    {
    {"speed_of_conveyor", 0 },
    {"heater_1", false },
    {"heater_2", false },
    {"heater_3", false },
    {"cooler", false },
    {"qc_camera_toggle", false },
    {"conveyor_manual_control", false},
    {"heater_1_manual_control", false},
    {"heater_2_manual_control", true},
    {"heater_3_manual_control", false},
    {"cooler_manual_control", false}
    };
    SUBCASE("Automatic loop heaters on period") {
        automatic_loop(sensor_input,ctrl_data,control_data_json,true,false);
        CHECK(ctrl_data.speed_of_conveyor == OPTIMAL_SOC);
        CHECK(ctrl_data.heaters == 0b00000101);
        CHECK(ctrl_data.cooler == 0b00000000);
    }
    SUBCASE("Automatic loop starting period") {
        automatic_loop(sensor_input,ctrl_data,control_data_json,true, true);
        CHECK(ctrl_data.speed_of_conveyor == 0);
        CHECK(ctrl_data.heaters == 0b00000101);
        CHECK(ctrl_data.cooler == 0b00000000);
    }
    SUBCASE("Automatic loop heaters off period") {
        automatic_loop(sensor_input,ctrl_data,control_data_json,false, false);
        CHECK(ctrl_data.speed_of_conveyor == OPTIMAL_SOC);
        CHECK(ctrl_data.heaters == 0b00000000);
        CHECK(ctrl_data.cooler == 0b00000000);
    }

    SUBCASE("Temperature = MAX_TEMP") {
        sensor_data sensor_input2{ 0, 0, MAX_TEMP, MAX_TEMP, MAX_TEMP, MAX_TEMP, MAX_TEMP,
        MAX_TEMP, MAX_TEMP, MAX_TEMP, MAX_TEMP, MAX_TEMP, 0};
        CHECK(highest_temp(sensor_input2) == MAX_TEMP);
        uint8_t test_heaters2;
        test_heaters2 = heating_control(sensor_input2, HEATER_1);
        test_heaters2 = heating_control(sensor_input2, HEATER_2);
        test_heaters2 = heating_control(sensor_input2, HEATER_3);
        CHECK(test_heaters2 == 0b00000111);
        CHECK(conveyor_control(sensor_input2) == OPTIMAL_SOC);
        CHECK(cooling_control(sensor_input2) == 1);
    }

    SUBCASE("Temperature > MAX_TEMP") {
        sensor_data sensor_input2{ 0, 0, MAX_TEMP + 1, MAX_TEMP + 1, MAX_TEMP + 1, MAX_TEMP + 1, MAX_TEMP + 1,
         MAX_TEMP + 1, MAX_TEMP + 1, MAX_TEMP + 1, MAX_TEMP + 1, MAX_TEMP + 1, 0};
        CHECK(highest_temp(sensor_input2) == MAX_TEMP + 1);
        uint8_t test_heaters2;
        test_heaters2 = heating_control(sensor_input2, HEATER_1);
        test_heaters2 = heating_control(sensor_input2, HEATER_2);
        test_heaters2 = heating_control(sensor_input2, HEATER_3);
        CHECK(test_heaters2 == 0b00000000);
        CHECK(conveyor_control(sensor_input2) == 0);
        CHECK(cooling_control(sensor_input2) == 1);
    }
}
