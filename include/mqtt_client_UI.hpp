#ifndef MQTT_CLIENT_UI_HPP
#define MQTT_CLIENT_UI_HPP

#include "mqtt/async_client.h"
#include "json.hpp"
#include <string>

// constexpr std::string ADDRESS = "5.tcp.eu.ngrok.io:18017";
const std::string ADDRESS_UI = "tcp://test.mosquitto.org:1883";
const std::string USER_ID_UI{ "control_sw_ui" }; // text UI uses this
const std::string TOPIC_REC_SENSOR = "sensor_control_data";
const std::string TOPIC_REC_CAMERA = "camera_data";
// const std::string TOPIC_RECEIVE = "conveyer_params"; // from UI
const std::string TOPIC_SEND_CONTROL = "test2_topic"; // from UI

using json = nlohmann::json;

class MQTT_Client_UI : public mqtt::callback
{
    public:
        MQTT_Client_UI(const std::string& address, const std::string& user_id);
        ~MQTT_Client_UI();

        void connect_broker();
        void disconnect_broker();
        void publish(const std::string& topic, const std::string& payload);
        void subscribe(const std::string& topic);
        virtual void message_arrived(mqtt::const_message_ptr msg) override;
        virtual void connection_lost(const std::string& cause) override;

        json camera_data;
        json sensor_data;
        
    private:
        mqtt::async_client client;

        

};


#endif
