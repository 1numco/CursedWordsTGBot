cmake_minimum_required(VERSION 3.14)
project(toxicity_classifier)

set(target toxicity_classifier)

# Найти Python и grpc_tools.protoc
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Настройка путей для Protobuf и gRPC
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/protobuf")
set(Protobuf_PROTOC_EXECUTABLE /usr/local/bin/protoc)

# Найти protoc и плагин для gRPC
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)

# Найти плагин для генерации gRPC кода
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

# Путь к .proto файлу
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.proto")

# Определить выходные файлы для C++ - В ИСХОДНОЙ ДИРЕКТОРИИ
set(PROTO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.cc")
set(PROTO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.h")
set(GRPC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.h")

# Определить выходные файлы для Python
set(PYTHON_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated_python")
file(MAKE_DIRECTORY ${PYTHON_OUTPUT_DIR})
set(PYTHON_PROTO_FILES
    "${PYTHON_OUTPUT_DIR}/toxicity_classifier_pb2.py"
    "${PYTHON_OUTPUT_DIR}/toxicity_classifier_pb2_grpc.py"
    "${PYTHON_OUTPUT_DIR}/toxicity_classifier_pb2.pyi"
)

# Добавить правило генерации кода для C++
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

# Добавить правило генерации кода для Python
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

# Создать статическую библиотеку
add_library(${target} STATIC
    client.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

# Добавить зависимость от генерации Python-кода
add_custom_target(generate_python_proto DEPENDS ${PYTHON_PROTO_FILES})
add_dependencies(${target} generate_python_proto)

target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # Указываем исходную директорию
)

# Связывание зависимостей
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