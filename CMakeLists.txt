cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(basic_data_signature_aquisition_protocol)

#################
###  Options  ###
#################


############################
###  Configured Headers  ###
############################

#####################################
###  Global Compiler Definitions  ###
#####################################

add_compile_options(/bigobj) #FIXME: May become a problem... needed for implementing CLI options

add_compile_definitions(NOMINMAX)
add_compile_definitions(_WEBSOCKETPP_CPP11_STL_)
add_compile_definitions(ASIO_STANDALONE)

##########################
###  Dependency Setup  ###
##########################

include("${CMAKE_CURRENT_SOURCE_DIR}/deps/asio.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/deps/websocketpp.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/deps/json.cmake")



# Protobufs
# Find the protobuf package, required if using protobuf
find_package(Protobuf REQUIRED)

# Include the protobuf headers
include_directories(${PROTOBUF_INCLUDE_DIRS})

# If your project uses any custom .proto files, you might also want to generate C++ sources from them
set(PROTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos)
set(PROTO_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto_sources)
set(PROTO_DESCRIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto_descriptors)
file(GLOB PROTO_FILES "${PROTOS_DIR}/*.proto")

# Define variables to hold all generated source and header files
set(GENERATED_SRCS)
set(GENERATED_HDRS)

include_directories(${PROTO_OUT_DIR})

message(${PROTO_FILES})

##########################
###  Global Variables  ###
##########################
set(INCLUDES_LIST 			
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/deps"
	"${websocketpp_INCLUDE_DIR}"
	"${json_INCLUDE_DIR}"
	"${asio_INCLUDE_DIR}"
	CACHE STRING "Status Monitor Includes List"
)

#################################
###  Compiler Specific Fixes  ###
#################################




########################
###  Custom Targets  ###
########################


# Automatically generate C++ sources from the .proto files
foreach(PROTO_FILE ${PROTO_FILES})
    message("Processing ${PROTO_FILE}")
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    set(GENERATED_SRC ${PROTO_OUT_DIR}/${PROTO_NAME}.pb.cc)
    set(GENERATED_HDR ${PROTO_OUT_DIR}/${PROTO_NAME}.pb.h)

    # Create custom commands for each proto file
    add_custom_command(
        OUTPUT ${GENERATED_SRC} ${GENERATED_HDR}
        COMMAND ${Protobuf_PROTOC_EXECUTABLE} 
        ARGS --cpp_out=${PROTO_OUT_DIR} --proto_path=${PROTOS_DIR} ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
        COMMENT "Generating ${GENERATED_SRC} and ${GENERATED_HDR}"
        VERBATIM
    )

    # Collect all generated files
    list(APPEND GENERATED_SRCS ${GENERATED_SRC})
    list(APPEND GENERATED_HDRS ${GENERATED_HDR})

    # Generate descriptor for each .proto file
    set(DESCRIPTOR_FILE ${PROTO_DESCRIP_DIR}/${PROTO_NAME}_descriptor.pb)
    add_custom_command(
        OUTPUT ${DESCRIPTOR_FILE}
        COMMAND ${Protobuf_PROTOC_EXECUTABLE} 
        ARGS --descriptor_set_out=${DESCRIPTOR_FILE} --include_imports --proto_path=${PROTOS_DIR} ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
        COMMENT "Generating descriptor for ${PROTO_NAME}"
        VERBATIM
    )
    
    # Collect all descriptor files
    list(APPEND ALL_DESCRIPTORS ${DESCRIPTOR_FILE})
endforeach()



# Define custom target that depends on all generated source and header files
add_custom_target(GenerateProtos ALL
    DEPENDS ${GENERATED_SRCS} ${GENERATED_HDRS}
    COMMENT "Custom target for generating protobuf sources"
)

# Define custom target for all descriptors
add_custom_target(GenerateDescriptors ALL
    DEPENDS ${ALL_DESCRIPTORS}
    COMMENT "Custom target for generating all protobuf descriptors"
)


########################
###  Subdirectories  ###
########################
# if(BUILD_TESTS) 
# 	#add_subdirectory(test)
# endif()
add_subdirectory(include)
# add_subdirectory(test)
