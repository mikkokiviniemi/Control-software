#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include "mqtt/async_client.h"
#include "json.hpp"
#include <string>
#include <vector>

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

        json input_control_data;
        bool json_handled{ true };
        
    private:
        mqtt::async_client client;

        

};


#endif
