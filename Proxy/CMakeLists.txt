cmake_minimum_required(VERSION 4.0)
project(ProxyServer LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++ -lc++abi")
set(CMAKE_SHARED_LINKER_FLAGS "-stdlib=libc++ -lc++abi")
set(CMAKE_STATIC_LINKER_FLAGS "-stdlib=libc++ -lc++abi")
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
find_package(Boost 1.89.0 REQUIRED url context)

set(SOURCE_EXE 
	${CMAKE_SOURCE_DIR}/source/Main.cc 
	${CMAKE_SOURCE_DIR}/source/AdamApi.cc 
	${CMAKE_SOURCE_DIR}/source/AdamService.cc 
	${CMAKE_SOURCE_DIR}/../gRPC/adam_api.grpc.pb.cc 
	${CMAKE_SOURCE_DIR}/../gRPC/adam_api.pb.cc
)

add_executable(ProxyServer.rl ${SOURCE_EXE})
target_include_directories(ProxyServer.rl PRIVATE 
	./include 
	../gRPC 
	../lib
	${GRPC_INCLUDE_DIRS} 
	${PROTOBUF_INCLUDE_DIRS} 
	${BOOST_INCLUDE_DIRS}
)

target_link_libraries(ProxyServer.rl PRIVATE 
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
