add_executable(cpp_serv 					"${CMAKE_CURRENT_SOURCE_DIR}/send.cpp" ${GENERATED_SRCS} ${PROTO_HDRS})
target_include_directories(cpp_serv PUBLIC "${INCLUDES_LIST}")
target_link_libraries(cpp_serv ${PROTOBUF_LIBRARIES})
