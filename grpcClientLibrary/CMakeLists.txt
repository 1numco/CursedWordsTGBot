cmake_minimum_required(VERSION 3.14)
project(toxicity_classifier)

set(target toxicity_classifier)

# Настройка путей для Protobuf и gRPC
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/protobuf")
set(Protobuf_PROTOC_EXECUTABLE /usr/local/bin/protoc)

# Поиск зависимостей
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)

# Источники и заголовки
set(PROTO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.cc)
set(PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.h)
set(GRPC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.cc)
set(GRPC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.h)

# Создаем статическую библиотеку
add_library(${target} STATIC client.cpp ${PROTO_SRCS} ${GRPC_SRCS})

target_include_directories(${target} PUBLIC .)

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