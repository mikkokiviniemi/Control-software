#include "mqtt_client.hpp"
#include "json_output.hpp"
#include "json.hpp"

#include <chrono>
#include <thread>

MQTT_Client::MQTT_Client(const std::string& address, const std::string& user_id) 
    : client(address,user_id) {
        client.set_callback(*this); // add a callback to the client to receive messages
    }

MQTT_Client::~MQTT_Client() { disconnect_broker(); }

void MQTT_Client::connect_broker() {
    mqtt::connect_options con_opts;
    try {
        client.connect(con_opts)->wait();
        std::cout << "connected to: " << client.get_server_uri() << '\n';
    } catch (const mqtt::exception& ex) {
        std::cerr << "Error: " << ex.what() << " ["
            << ex.get_reason_code() << "]" << '\n';
    }
}

void MQTT_Client::disconnect_broker() {
    client.disconnect()->wait();
    std::cout << "disconnected\n";
}

void MQTT_Client::publish(const std::string& topic, const std::string& payload){
    mqtt::message_ptr msg = mqtt::make_message(topic, payload);
    msg->set_qos(0);

    try {
        client.publish(msg)->wait();
    } catch (const mqtt::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void MQTT_Client::subscribe(const std::string& topic){
    try {
        client.subscribe(topic, 0)->wait();
        std::cout << "subscribed to topic " << topic << '\n';
    } catch (const mqtt::exception& e){
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void MQTT_Client::message_arrived(mqtt::const_message_ptr msg) {
    // std::string topic = msg->get_topic();

    while (!json_handled) {
        std::cout << "waiting for message to be read...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    input_control_data = json::parse(msg->get_payload_str());
    json_handled = true;
    // std::cout << "Message arrived! Topic: " << topic << " , payload: " << msg->get_payload_str() << '\n';

}

// a function that the callback uses
void MQTT_Client::connection_lost(const std::string& cause) {
    std::cout << "\nConnection lost" << '\n';
    if (!cause.empty())
        std::cout << "\tcause: " << cause << '\n';
}
