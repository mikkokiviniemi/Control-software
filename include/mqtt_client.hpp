#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include "mqtt/async_client.h"
#include "json.hpp"
#include <string>

inline const std::string ADDRESS = "4.tcp.eu.ngrok.io:17857";
// inline const std::string ADDRESS = "tcp://test.mosquitto.org:1883";
inline const std::string USER_ID_CONTROL = "control_sw";
inline const std::string TOPIC_SEND_SENSOR = "sensor_control_data";
inline const std::string TOPIC_RECEIVE = "conveyor_params"; // from UI
// inline const std::string TOPIC_RECEIVE = "test2_topic"; // from UI

using json = nlohmann::json;

class MQTT_Client : public mqtt::callback
{
    public:
        MQTT_Client(const std::string& address, const std::string& user_id);
        ~MQTT_Client();

        void connect_broker();
        void disconnect_broker();
        void publish(const std::string& topic, const std::string& payload);
        void subscribe(const std::string& topic);
        virtual void message_arrived(mqtt::const_message_ptr msg) override;
        virtual void connection_lost(const std::string& cause) override;

        json input_control_data = {
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
        
    private:
        mqtt::async_client client;

        

};


#endif
