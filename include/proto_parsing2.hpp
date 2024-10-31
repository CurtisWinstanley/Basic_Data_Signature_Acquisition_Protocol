#include <google/protobuf/util/json_util.h>
#include <google/protobuf/io/zero_copy_stream_impl.h> // for FileInputStream
#include <fstream>
#include "../proto_sources/my_message.pb.h"

std::string protoToJson(const std::string& protoFileName) {
    // Assuming you have a message type MyMessage
    AddressBook msg;

    // Read the binary proto file
    std::ifstream file(protoFileName, std::ios::binary);
    if (!msg.ParseFromIstream(&file)) {
        std::cerr << "Failed to parse the protobuf file." << std::endl;
        return "";
    }

    std::string jsonOutput;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;  // Pretty-print
    options.always_print_primitive_fields = true;  // Optional: print all fields even if default

    auto status = google::protobuf::util::MessageToJsonString(msg, &jsonOutput, options);
    if (!status.ok()) {
        std::cerr << "Failed to convert protobuf to JSON: " << int(status.code()) << std::endl;
        return "";
    }

    return jsonOutput;
}
