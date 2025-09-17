cmake_minimum_required(VERSION 4.0)
project(TasksAlgoritm LANGUAGES CXX)

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

find_package(OpenSSL REQUIRED)

set(CMAKE_PREFIX_PATH "/usr/local${CMAKE_PREFIX_PATH}")
set(CMAKE_LIBRARY_PATH "/usr/local/lib${CMAKE_LIBRARY_PATH}")
set(CMAKE_INCLUDE_PATH "/usr/local/include${CMAKE_INCLUDE_PATH}")


find_package(absl REQUIRED)
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

set(SOURCE_EXE 
	./source/Main.cc
  ./source/Tasks.cc
  ./source/TasksService.cc
  ../gRPC/tasks_api.pb.cc
  ../gRPC/tasks_api.grpc.pb.cc
)

add_executable(TasksServer ${SOURCE_EXE})
set(SOURCE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../release/libMathPlot.so)
target_include_directories(TasksServer PRIVATE 
  ./include 
  ../lib 
  ../gRPC 
  ${GRPC_INCLUDE_DURS}
  ${PROTOBUF_INCLUDE_DIRS}
)
target_link_libraries(TasksServer PRIVATE
	${SOURCE_LIB}
  ${OpenSSL_LIBRARIES}
  absl::strings
  gRPC::grpc++
  gRPC::grpc++_reflection
  protobuf::libprotobuf
)
