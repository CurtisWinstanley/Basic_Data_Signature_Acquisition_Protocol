#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>  // For JSON handling
#include <functional>  // For std::bind and std::placeholders

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/text_format.h>  // Include TextFormat

using json = nlohmann::json;




typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "Received a request to send proto file in JSON format." << std::endl;

    // Convert .proto file content to JSON
    nlohmann::json json_data = protoToJson("proto_descriptors\\my_message_descriptor.pb");
    
    // Serialize JSON to string
    std::string json_string = json_data.dump(4);
    std::cout << json_string << std::endl;

    // Send the JSON string as a response
    try {
        s->send(hdl, json_string, websocketpp::frame::opcode::text);
    } catch (const websocketpp::lib::error_code& e) {
        std::cerr << "Error sending message: " << e.message() << std::endl;
    }
}

int main() {
    server proto_server;


    try {
        proto_server.set_message_handler(bind(&on_message, &proto_server, std::placeholders::_1, std::placeholders::_2));
        proto_server.init_asio();
        proto_server.listen(9002);
        proto_server.start_accept();

        proto_server.run();
    } catch (websocketpp::exception const & e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Other exception" << std::endl;
    }
}
