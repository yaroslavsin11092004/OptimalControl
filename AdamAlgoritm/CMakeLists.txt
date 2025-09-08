cmake_minimum_required(VERSION 4.0)
project(AdamServer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++ -lc++abi")
set(CMAKE_STATIC_LINKER_FLAGS "-stdlib=libc++ -lc++abi")
set(CMAKE_SHARED_LINKER_FLAGS "-stdlib=libc++ -lc++abi")
set(CMAKE_MODULE_LINKER_FLAGS "-stdlib=libc++ -lc++abi")

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	message(WARNING "Not using clang compiler")
endif()

find_path(PROTOBUF_INCLUDE_DIR google/protobuf/message.h PATHS /usr/local/include)
find_library(PROTOBUF_LIBRARY NAMES protobuf PATHS /usr/local/lib)

find_path(GRPC_INCLUDE_DIR grpcpp/grpcpp.h PATHS /usr/local/include)
find_library(GRPC_LIBRARY NAMES grpc++ PATHS /usr/local/lib)
find_library(GRPC_CORE_LIBRARY NAMES grpc PATHS /usr/local/lib)

if(NOT PROTOBUF_INCLUDE_DIR OR NOT PROTOBUF_LIBRARY)
    message(FATAL_ERROR "Protobuf not found in /usr/local")
endif()

if(NOT GRPC_INCLUDE_DIR OR NOT GRPC_LIBRARY OR NOT GRPC_CORE_LIBRARY)
    message(FATAL_ERROR "gRPC not found in /usr/local")
endif()

message(STATUS "Found Protobuf: ${PROTOBUF_LIBRARY}")
message(STATUS "Found gRPC++: ${GRPC_LIBRARY}")
message(STATUS "Found gRPC core: ${GRPC_CORE_LIBRARY}")

set(CMAKE_PREFIX_PATH "/usr/local/torch_local${CMAKE_PREFIX_PATH}")
set(CMAKE_LIBRARY_PATH "/usr/local/torch_local/lib${CMAKE_LIBRARY_PATH}")
set(CMAKE_INCLUDE_PATH "/usr/local/torch_local/include${CMAKE_INCLUDE_PATH}")
find_package(Torch REQUIRED)

set(SOURCE_EXE 
	./source/Main.cc
  ./source/Adam.cc
)
add_executable(AdamServer ${SOURCE_EXE})
target_include_directories(AdamServer PRIVATE
     /usr/local/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../gRPC/
    ${CMAKE_SOURCE_DIR}/../lib
)
target_include_directories(AdamServer SYSTEM PRIVATE 
    ${TORCH_INCLUDE_DIRS}
)
target_link_libraries(AdamServer PRIVATE 
  ${PROTOBUF_LIBRARY}
  ${GRPC_LIBRARY}
  ${GRPC_CORE_LIBRARY}
	${TORCH_LIBRARIES}
)
