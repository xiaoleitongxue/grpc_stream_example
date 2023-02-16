cmake_minimum_required(VERSION 3.5.1)

project(stream_test C CXX)

include(../cmake/common.cmake)

# Proto file
get_filename_component(st_proto "./protos/stream_test.proto" ABSOLUTE)
get_filename_component(st_proto_path "${st_proto}" PATH)

# Generated sources
set(st_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/stream_test.pb.cc")
set(st_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/stream_test.pb.h")
set(st_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/stream_test.grpc.pb.cc")
set(st_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/stream_test.grpc.pb.h")
add_custom_command(
      OUTPUT "${st_proto_srcs}" "${st_proto_hdrs}" "${st_grpc_srcs}" "${st_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${st_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${st_proto}"
      DEPENDS "${st_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# rg_grpc_proto
add_library(st_grpc_proto
  ${st_grpc_srcs}
  ${st_grpc_hdrs}
  ${st_proto_srcs}
  ${st_proto_hdrs})
target_link_libraries(st_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})


# Targets route_guide_(client|server)
foreach(_target
  client server)
  add_executable(${_target}
    "${_target}.cc")
  target_link_libraries(${_target}
    st_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()