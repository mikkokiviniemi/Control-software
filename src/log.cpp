#include "log.hpp"
#include "json_output.hpp"
#include "json.hpp"

#include <chrono>

MQTT_Client_Log::MQTT_Client_Log(const std::string& address, const std::string& user_id) 
    : client(address,user_id) {
        client.set_callback(*this); // add a callback to the client to receive messages
    }

MQTT_Client_Log::~MQTT_Client_Log() { disconnect_broker(); }

void MQTT_Client_Log::connect_broker() {
    mqtt::connect_options con_opts;
    con_opts.set_clean_session(false);
    try {
        client.connect(con_opts)->wait();
        std::cout << "connected to: " << client.get_server_uri() << '\n';
    } catch (const mqtt::exception& ex) {
        std::cerr << "Error: " << ex.what() << " ["
            << ex.get_reason_code() << "]" << '\n';
    }
}

void MQTT_Client_Log::disconnect_broker() {
    client.disconnect()->wait();
    std::cout << "disconnected\n";
}

/* void MQTT_Client_Log::publish(const std::string& topic, const std::string& payload){
    mqtt::message_ptr msg = mqtt::make_message(topic, payload);
    msg->set_qos(0);

    try {
        client.publish(msg)->wait();
    } catch (const mqtt::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
    }
} */

void MQTT_Client_Log::subscribe(const std::string& topic){
    try {
        client.subscribe(topic, 0)->wait();
        std::cout << "subscribed to topic " << topic << '\n';
    } catch (const mqtt::exception& e){
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void MQTT_Client_Log::message_arrived(mqtt::const_message_ptr msg) {
    std::string topic = msg->get_topic();

    if (topic == TOPIC_LOG_SENSOR) {
        std::cout << "Sensor data received:\n"; 
        print_sensor_log(json::parse(msg->get_payload_str()));
    }

    if (topic == TOPIC_LOG_CONTROL) {
        std::cout << "Control data received:\n";
        print_control_log(json::parse(msg->get_payload_str()));
    }
}

// a function that the callback uses
void MQTT_Client_Log::connection_lost(const std::string& cause) {
    std::cout << "\nConnection lost" << '\n';
    if (!cause.empty())
        std::cout << "\tcause: " << cause << '\n';
}

void print_sensor_log(const json& sensor_data_json)
{
    std::cout << "conveyor: " << sensor_data_json["speed_of_conveyor"] << '\n';
    std::cout << "heater_1: " << sensor_data_json["heater_1"] << '\n';
    std::cout << "heater_2: " << sensor_data_json["heater_2"] << '\n';
    std::cout << "heater_3: " << sensor_data_json["heater_3"] << '\n';
    std::cout << "cooler: " << sensor_data_json["cooler"] << '\n';
    std::cout << "qc_camera_toggle: " << sensor_data_json["qc_camera_toggle"] << '\n';
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

void print_control_log(const json& control_data_json)
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

int main() {
    MQTT_Client_Log client(ADDRESS_LOG, USER_ID_LOG);
    client.connect_broker();
    client.subscribe(TOPIC_LOG_SENSOR);
    client.subscribe(TOPIC_LOG_CONTROL);
    std::cout << "Printing log, press q to quit\n";

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "q") {
            return 0;
        }
    }
}