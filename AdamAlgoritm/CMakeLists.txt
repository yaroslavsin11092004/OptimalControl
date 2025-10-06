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


set(CMAKE_PREFIX_PATH "/usr/local/torch_local${CMAKE_PREFIX_PATH}")
set(CMAKE_LIBRARY_PATH "/usr/local/torch_local/lib${CMAKE_LIBRARY_PATH}")
set(CMAKE_INCLUDE_PATH "/usr/local/torch_local/include${CMAKE_INCLUDE_PATH}")
find_package(Torch REQUIRED)

set(CMAKE_PREFIX_PATH "/usr/local${CMAKE_PREFIX_PATH}")
set(CMAKE_INCLUDE_PATH "/usr/local/include${CMAKE_INCLUDE_PATH}")
set(CMAKE_LIBRARY_PATH "/usr/local/lib${CMAKE_LIBRARY_PATH}")
find_package(Boost 1.89.0 REQUIRED url context)
find_package(absl REQUIRED)
set(SOURCE_EXE 
	${CMAKE_SOURCE_DIR}/source/Main.cc
	${CMAKE_SOURCE_DIR}/source/Adam.cc 
	${CMAKE_SOURCE_DIR}/source/Router.cc 
	${CMAKE_SOURCE_DIR}/source/Server.cc
	${CMAKE_SOURCE_DIR}/source/Tokenizer.cc 
	${CMAKE_SOURCE_DIR}/source/HamiltonBuilder.cc
)

add_executable(AdamServer.rl ${SOURCE_EXE})

target_include_directories(AdamServer.rl PRIVATE 
	${BOOST_INCLUDE_DIRS}
	${TORCH_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include 
	${CMAKE_SOURCE_DIR}/../lib
)

target_link_libraries(AdamServer.rl PRIVATE 
	Boost::boost
	Boost::url
	absl::flat_hash_map
	${TORCH_LIBRARIES}
)
