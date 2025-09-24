cmake_minimum_required(VERSION 4.0)
project(CoreAlgoritm LANGUAGES CXX)

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
find_package(Boost 1.89.0 REQUIRED url context)
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

include_directories(
	./include/
	../lib
  ../gRPC/
)

set(SOURCE_EXE 
	./source/Main.cc
	./source/Router.cc
	./source/Server.cc
	./source/OptContrAlg.cc
  ../gRPC/adam_api.pb.cc
  ../gRPC/adam_api.grpc.pb.cc
  ../gRPC/tasks_api.pb.cc
  ../gRPC/tasks_api.grpc.pb.cc
  ./source/AdamApiRpc.cc
  ./source/TasksApiRps.cc
)

set(SOURCE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../release/libMathPlot.so)

add_executable(CoreServer.rl ${SOURCE_EXE})

target_link_libraries(CoreServer.rl 
	${SOURCE_LIB}
  ${OpenSSL_LIBRARIES}
  Boost::boost
  Boost::url
  absl::strings
  absl::base 
  absl::flags
  gRPC::grpc++
  gRPC::grpc++_reflection
  protobuf::libprotobuf
)
