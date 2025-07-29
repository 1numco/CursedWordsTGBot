
project(server_grpc)

find_package(Python COMPONENTS Interpreter Development REQUIRED)

set(PROTO_FILE "${CMAKE_SOURCE_DIR}/ClassifierClient/grpc/")
set(PYTHON_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")

file(MAKE_DIRECTORY ${PYTHON_OUTPUT_DIR})
set(PYTHON_PROTO_FILES
    "${PYTHON_OUTPUT_DIR}/toxicity_classifier_pb2.py"
    "${PYTHON_OUTPUT_DIR}/toxicity_classifier_pb2_grpc.py"
)

add_custom_command(
    OUTPUT ${PYTHON_PROTO_FILES}
    COMMAND ${Python_EXECUTABLE} -m grpc_tools.protoc
        -I "${PROTO_FILE}"
        --python_out "${PYTHON_OUTPUT_DIR}"
        --grpc_python_out "${PYTHON_OUTPUT_DIR}"
        "${PROTO_FILE}/toxicity_classifier.proto"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/ClassifierClient/grpc"
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating gRPC Python code"
)

add_custom_target(generate_python_proto ALL DEPENDS ${PYTHON_PROTO_FILES})