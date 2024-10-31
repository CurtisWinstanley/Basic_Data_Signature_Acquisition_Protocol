// const protobuf = require("protobufjs");
// const WebSocket = require("ws");

// Assume the server is sending a .proto schema as a message
// const ws = new WebSocket("ws://your-websocket-server");

// ws.on("message", (protoSchemaString) => {
//   try {
//     // Parse the .proto schema string
//     const root = protobuf.parse(protoSchemaString).root;

//     // Convert the root descriptor (schema) to a JSON object
//     const schemaJson = root.toJSON();

//     // Serialize the schema into a JSON string with formatting
//     const schemaString = JSON.stringify(schemaJson, null, 2);

//     console.log("Proto Schema as JSON:\n", schemaString);

//     // Optionally save the JSON schema to a file
//     require("fs").writeFileSync("schema.json", schemaString, (writeErr) => {
//       if (writeErr) console.error("Error writing JSON file:", writeErr);
//       else console.log("Schema JSON saved to schema.json");
//     });
//   } catch (err) {
//     console.error("Error parsing .proto schema string:", err);
//   }
// });

const protobuf = require("protobufjs");
const fs = require("fs");

const protoSchemaString = `syntax = "proto3";

message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 results_per_page = 3;
}`;

try {
    // Parse the .proto schema string
    const root = protobuf.parse(protoSchemaString).root;

    // Convert the root descriptor (schema) to a JSON object
    const schemaJson = root.toJSON();

    // Serialize the schema into a JSON string with formatting
    const schemaString = JSON.stringify(schemaJson, null, 2);

    console.log("Proto Schema as JSON:\n", schemaString);

    // Optionally save the JSON schema to a file
    fs.writeFileSync("schema.json", schemaString);
    console.log("Schema JSON saved to schema.json");
} catch (err) {
    console.error("Error parsing .proto schema string:", err);
}

