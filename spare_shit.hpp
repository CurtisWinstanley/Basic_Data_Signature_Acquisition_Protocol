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
using namespace google::protobuf;
using namespace google::protobuf::compiler;



class ErrorCollector : public MultiFileErrorCollector {
public:
    void AddError(const std::string& filename, int line, int column, const std::string& message) override {
        std::cerr << "Error in file " << filename << " at line " << line << ", column " << column << ": " << message << std::endl;
    }
};

json extractFieldDescriptor(const FieldDescriptor* field) {
    json fieldJson;
    fieldJson["name"] = field->name();
    fieldJson["number"] = field->number();
    fieldJson["label"] = field->is_repeated() ? "repeated" : "optional";
    fieldJson["type"] = FieldDescriptor::TypeName(field->type());
    if (field->type() == FieldDescriptor::TYPE_MESSAGE) {
        fieldJson["message_type"] = field->message_type()->full_name();
    }
    return fieldJson;
}

json extractMessageDescriptor(const Descriptor* message) {
    json messageJson;
    messageJson["name"] = message->name();
    messageJson["fields"] = json::array();

    for (int i = 0; i < message->field_count(); ++i) {
        const FieldDescriptor* field = message->field(i);
        messageJson["fields"].push_back(extractFieldDescriptor(field));
    }

    return messageJson;
}

int main() {
    // Load proto file into a string
    std::ifstream protoFile("protos/my_message.proto");
    if (!protoFile) {
        std::cerr << "Failed to open my_message.proto." << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << protoFile.rdbuf();
    protoFile.close();
    std::string protoContent = buffer.str();

    // Parse the proto content directly into a FileDescriptorProto
    FileDescriptorProto fileProto;
    if (!TextFormat::ParseFromString(protoContent, &fileProto)) {
        std::cerr << "Failed to parse proto content into descriptor." << std::endl;
        return 1;
    }

    // Build the descriptor from the FileDescriptorProto
    DescriptorPool descriptorPool;
    const FileDescriptor* fileDescriptor = descriptorPool.BuildFile(fileProto);
    if (!fileDescriptor) {
        std::cerr << "Failed to build file descriptor from proto content." << std::endl;
        return 1;
    }

    // Process descriptors
    json schemaJson;
    schemaJson["messages"] = json::array();

    for (int i = 0; i < fileDescriptor->message_type_count(); ++i) {
        const Descriptor* messageDescriptor = fileDescriptor->message_type(i);
        schemaJson["messages"].push_back(extractMessageDescriptor(messageDescriptor));
    }

    // Output JSON schema
    std::cout << schemaJson.dump(2) << std::endl;

    // Optionally, save the schema to a file
    std::ofstream outFile("schema.json");
    outFile << schemaJson.dump(2);
    outFile.close();

    return 0;
}


// typedef websocketpp::server<websocketpp::config::asio> server;

// void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
//     std::cout << "Received a request to send proto file in JSON format." << std::endl;

//     // Convert .proto file content to JSON
//     nlohmann::json json_data = protoToJson("proto_descriptors\\my_message_descriptor.pb");
    
//     // Serialize JSON to string
//     std::string json_string = json_data.dump(4);
//     std::cout << json_string << std::endl;

//     // Send the JSON string as a response
//     try {
//         s->send(hdl, json_string, websocketpp::frame::opcode::text);
//     } catch (const websocketpp::lib::error_code& e) {
//         std::cerr << "Error sending message: " << e.message() << std::endl;
//     }
// }

// int main() {
//     server proto_server;

//     // Convert .proto file content to JSON
//     nlohmann::json json_data = protoToJson("proto_descriptors\\my_message_descriptor.pb");
//     // Serialize JSON to string
//     std::string json_string = json_data.dump(4);
//     std::cout << json_string << std::endl;

//     try {
//         proto_server.set_message_handler(bind(&on_message, &proto_server, std::placeholders::_1, std::placeholders::_2));
//         proto_server.init_asio();
//         proto_server.listen(9002);
//         proto_server.start_accept();

//         proto_server.run();
//     } catch (websocketpp::exception const & e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     } catch (...) {
//         std::cerr << "Other exception" << std::endl;
//     }
// }
