const protobuf = require("protobufjs");
const fs = require("fs");

// Load the .proto file and convert it to JSON schema
protobuf.load("../protos/my_message.proto", (err, root) => {
  if (err) {
    console.error("Error loading .proto file:", err);
    return;
  }

  // Convert the root descriptor (schema) to a JSON object
  const schemaJson = root.toJSON();

  // Serialize the schema into a JSON string with formatting
  const schemaString = JSON.stringify(schemaJson, null, 2);

  console.log("Proto Schema as JSON:\n", schemaString);

  // Save the schema JSON to a file (optional)
  fs.writeFileSync("schema.json", schemaString, (writeErr) => {
    if (writeErr) console.error("Error writing JSON file:", writeErr);
    else console.log("Schema JSON saved to schema.json");
  });
});