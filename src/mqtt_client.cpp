#include "mqtt_client.hpp"
#include "json_output.hpp"

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
    msg->set_qos(1);

    try {
        client.publish(msg)->wait();
    } catch (const mqtt::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void MQTT_Client::subscribe(const std::string& topic){
    try {
        client.subscribe(topic, 1)->wait();
        std::cout << "subscribed to topic " << topic << '\n';
    } catch (const mqtt::exception& e){
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void MQTT_Client::message_arrived(mqtt::const_message_ptr msg) {
    std::string payload = msg->get_payload_str();
    std::string topic = msg->get_topic();

    // json json_UI_input = json::parse(payload);

    std::cout << "Message arrived! Topic: " << topic << " , payload: " << payload << '\n';

    // what to do with the data?
    // if (topic == TOPIC_RECEIVE) {

}

// a function that the callback uses
void MQTT_Client::connection_lost(const std::string& cause) {
    std::cout << "\nConnection lost" << '\n';
    if (!cause.empty())
        std::cout << "\tcause: " << cause << '\n';
}








