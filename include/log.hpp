#ifndef LOG_HPP
#define LOG_HPP

#include "mqtt/async_client.h"
#include "json.hpp"
#include <string>
#include "text_ui.hpp"

// const std::string ADDRESS_LOG = "4.tcp.eu.ngrok.io:17857";
const std::string ADDRESS_LOG = "tcp://test.mosquitto.org:1883";
const std::string USER_ID_LOG{ "control_sw_log" }; // text UI uses this
const std::string TOPIC_LOG_SENSOR = "sensor_control_data1";
const std::string TOPIC_LOG_CONTROL = "conveyor_params"; // from UI

using json = nlohmann::json;

class MQTT_Client_Log : public mqtt::callback
{
    public:
        MQTT_Client_Log(const std::string& address, const std::string& user_id);
        ~MQTT_Client_Log();

        void connect_broker();
        void disconnect_broker();
        // void publish(const std::string& topic, const std::string& payload);
        void subscribe(const std::string& topic);
        virtual void message_arrived(mqtt::const_message_ptr msg) override;
        virtual void connection_lost(const std::string& cause) override;
        
    private:
        mqtt::async_client client;

};

void print_control_log(const json& control_data_json);
void print_sensor_log(const json& sensor_data_json);

#endif
