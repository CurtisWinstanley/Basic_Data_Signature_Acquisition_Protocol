#include <fstream>
#include <iostream>
#include <string>
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/compiler/importer.h"
#include <google/protobuf/util/json_util.h>
#include "nlohmann/json.hpp"  // Use the nlohmann/json library for convenient JSON handling.

using namespace google::protobuf;
using json = nlohmann::json;

std::string protoToJSON(const std::string& descriptorPath) {
    // Load the binary descriptor
    FileDescriptorSet fileSet;
    std::fstream input(descriptorPath, std::ios::in | std::ios::binary);
    if (!fileSet.ParseFromIstream(&input)) {
        std::cerr << "Failed to parse file descriptor!" << std::endl;
        return "";
    }

    json j;
    // Iterate over all files in the file set (assuming single file for simplicity)
    for (int i = 0; i < fileSet.file_size(); ++i) {
        const FileDescriptorProto& fileProto = fileSet.file(i);
        json jFile;
        jFile["name"] = fileProto.name();
        
        // Check if package is set and not empty before adding to JSON
        if (fileProto.package().size() != 0) {
            jFile["package"] = fileProto.package();
        } else {
            jFile["package"] = "default_package";  // Or handle it by setting a default or skipping
        }

        // Messages
        json jMessages = json::array();
        for (int m = 0; m < fileProto.message_type_size(); ++m) {
            const DescriptorProto& messageProto = fileProto.message_type(m);
            json jMessage;
            jMessage["name"] = messageProto.name();

            // Fields
            json jFields = json::array();
            for (int f = 0; f < messageProto.field_size(); ++f) {
                const FieldDescriptorProto& fieldProto = messageProto.field(f);
                json jField;
                jField["name"] = fieldProto.name();
                jField["type"] = fieldProto.type_name();  // Ensure type_name is valid or provide defaults
                jFields.push_back(jField);
            }
            jMessage["fields"] = jFields;
            jMessages.push_back(jMessage);
        }
        jFile["messages"] = jMessages;

        j.push_back(jFile);
    }

    return j.dump(4);  // Pretty print with indent of 4 spaces
}
