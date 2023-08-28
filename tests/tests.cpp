#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "data_structures.hpp"
#include "dummy_data.hpp"
#include "input_validation.hpp"
#include "automatic_controls.hpp"
#include "mqtt_client.hpp"
#include "mqtt/async_client.h"

TEST_CASE("Dummy data")
{

    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    control_data ctrl_data{ 150, 0b111, 1, 1 };

    dummy_data_generator(sensor_input, ctrl_data);

    CHECK(sensor_input.speed_of_conveyor == 1);   
    CHECK(sensor_input.temp_sensor01 == sensor_input.temp_sensor10);
    CHECK(sensor_input.temp_sensor01 == 273);
}

TEST_CASE("Data validation")
{

    sensor_data sensor_input{ 0, 0, 1250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    control_data ctrl_data{150, 0b111, 1, 1 };
    
    uint16_t failed_sensor_input_validation{ temperature_input_validation(sensor_input) };
    uint8_t failed_control_input_validation{ control_input_validation(ctrl_data) };

    CHECK((failed_sensor_input_validation & FAILED_TEMP_SENSOR01));
    CHECK(failed_control_input_validation == 0);

}

TEST_CASE("Control software")
{
     
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
    {"qc_camera_status", false },
    {"conveyor_manual_control", true},
    {"heater1_manual_control", true},
    {"heater2_manual_control" , true},
    {"heater3_manual_control", true},
    {"cooler_manual_control", true}
    };

    const std::string ADDRESS = "tcp://test.mosquitto.org:1883";
    const std::string TEST_ID_1 = "control_sw_1";
    const std::string TEST_ID_2 = "user";
    const std::string TOPIC_SEND_SENSOR = "sensor_control_data";
    const std::string TOPIC_SEND_CAMERA = "camera_data";

    MQTT_Client test_client_sender (ADDRESS, TEST_ID_1);
    MQTT_Client test_client_user (ADDRESS, TEST_ID_2);
    test_client_sender.connect_broker();
    test_client_user.connect_broker();

    test_client_user.subscribe(TOPIC_SEND_SENSOR);
    test_client_sender.publish(TOPIC_SEND_SENSOR, control_data_json.dump());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    CHECK(test_client_user.input_control_data == json::parse(control_data_json.dump()));
    CHECK(test_client_user.json_handled == true);

}
