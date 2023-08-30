#include "log.hpp"
#include "json_output.hpp"
#include "json.hpp"

#include <iomanip>

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
    try
    {
        client.disconnect()->wait();
        std::cout << "disconnected\n";
    }
    catch(const mqtt::exception& e)
    {
        std::cerr << "Couldn't disconnect: " << e.what() << '\n';
    }
}

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
    float sensor1{ sensor_data_json["temp_sensors"][0] };
    float sensor2{ sensor_data_json["temp_sensors"][1] };
    float sensor3{ sensor_data_json["temp_sensors"][2] };
    float sensor4{ sensor_data_json["temp_sensors"][3] };
    float sensor5{ sensor_data_json["temp_sensors"][4] };
    float sensor6{ sensor_data_json["temp_sensors"][5] };
    float sensor7{ sensor_data_json["temp_sensors"][6] };
    float sensor8{ sensor_data_json["temp_sensors"][7] };
    float sensor9{ sensor_data_json["temp_sensors"][8] };
    float sensor10{ sensor_data_json["temp_sensors"][9] };

    std::cout << std::fixed << std::setprecision(1);
    
    std::cout << "conveyor:\t\t" << sensor_data_json["speed_of_conveyor"] << '\n';
    std::cout << "heater_1:\t\t" << sensor_data_json["heater_1"] << '\n';
    std::cout << "heater_2:\t\t" << sensor_data_json["heater_2"] << '\n';
    std::cout << "heater_3:\t\t" << sensor_data_json["heater_3"] << '\n';
    std::cout << "cooler:\t\t\t" << sensor_data_json["cooler"] << '\n';
    std::cout << "temperatures:\t\t";
    std::cout << sensor1 << ' ';
    std::cout << sensor2 << ' ';
    std::cout << sensor3 << ' ';
    std::cout << sensor4 << ' ';
    std::cout << sensor5 << '\n';
    std::cout << "\t\t\t";
    std::cout << sensor6 << ' ';
    std::cout << sensor7 << ' ';
    std::cout << sensor8 << ' ';
    std::cout << sensor9 << ' ';
    std::cout << sensor10 << '\n';
    std::cout << "qc_camera_fails:\t" << sensor_data_json["qc_camera_fails"] << '\n';
    std::cout << "failures:\t\t" << sensor_data_json["failures"] << '\n';
    std::cout << "timestamp:\t\t" << sensor_data_json["time_stamp"] << '\n' << std::endl;
}

void print_control_log(const json& control_data_json)
{
    
    std::cout << std::right << std::setw(75) << "Control data received:\n";
    std::cout << std::right << std::setw(75) <<  "Conveyor target speed: " << control_data_json["speed_of_conveyor"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Heater1 on/off: "         << control_data_json["heater_1"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Heater2 on/off: "         << control_data_json["heater_2"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Heater3 on/off: "         << control_data_json["heater_3"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Cooler on/off: "          << control_data_json["cooler"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Camera on/off: "          << control_data_json["qc_camera_toggle"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Conveyor manual control: "<< control_data_json["conveyor_manual_control"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Heater1 manual control: " << control_data_json["heater_1_manual_control"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Heater2 manual control: " << control_data_json["heater_2_manual_control"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Heater3 manual control: " << control_data_json["heater_3_manual_control"] << '\n';
    std::cout << std::right << std::setw(75) <<  "Cooler manual control: "  << control_data_json["cooler_manual_control"] << '\n' << std::endl;
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
