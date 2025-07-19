cmake_minimum_required(VERSION 3.14)
project(toxicity_classifier)

set(target toxicity_classifier)

find_package(Python COMPONENTS Interpreter Development REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/protobuf")
set(Protobuf_PROTOC_EXECUTABLE /usr/local/bin/protoc)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)


find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.proto")

set(PROTO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.cc")
set(PROTO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.h")
set(GRPC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.h")

set(PYTHON_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/server_grpc")
file(MAKE_DIRECTORY ${PYTHON_OUTPUT_DIR})
set(PYTHON_PROTO_FILES
    "${PYTHON_OUTPUT_DIR}/toxicity_classifier_pb2.py"
    "${PYTHON_OUTPUT_DIR}/toxicity_classifier_pb2_grpc.py"
)

add_custom_command(
    OUTPUT ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
         --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
         -I "${CMAKE_CURRENT_SOURCE_DIR}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
         ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating gRPC C++ code"
)

add_custom_command(
    OUTPUT ${PYTHON_PROTO_FILES}
    COMMAND ${Python_EXECUTABLE} -m grpc_tools.protoc
        -I "${CMAKE_CURRENT_SOURCE_DIR}"
        --python_out "${PYTHON_OUTPUT_DIR}"
        --grpc_python_out "${PYTHON_OUTPUT_DIR}"
        --pyi_out "${PYTHON_OUTPUT_DIR}"
        ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating gRPC Python code"
)

add_library(${target} STATIC
    client.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

add_custom_target(generate_python_proto DEPENDS ${PYTHON_PROTO_FILES})
add_dependencies(${target} generate_python_proto)

target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${target}
    PUBLIC
    absl::cord
    absl::cord_internal
    absl::base
    absl::strings
    absl::log_internal_check_op
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    utf8_range::utf8_validity
)