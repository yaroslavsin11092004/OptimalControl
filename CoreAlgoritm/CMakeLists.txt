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

set(BOOST_ROOT "/usr/local")
set(BOOST_INCLUDE_DIR "${BOOST_ROOT}/include/boost-1_89")
set(BOOST_LIB_DIR "${BOOST_ROOT}/lib/")


find_library(BOOST_URL_LIB 
    NAMES boost_url-clang20-mt-d-x64-1_89 boost_url
    PATHS ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
)

find_library(BOOST_COROUTINE_LIB 
    NAMES boost_coroutine boost_coroutine-clang20-mt-d-x64-1_89
    PATHS ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
)

find_library(BOOST_CONTEXT_LIB 
    NAMES boost_context boost_context-clang20-mt-d-x64-1_89
    PATHS ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
)

include_directories(
	./include/
	../lib
	${BOOST_INCLUDE_DIR}
)

set(SOURCE_EXE 
	./source/Main.cpp
	./source/Router.cpp
	./source/Server.cpp
	./source/AdamApi.cpp
	./source/OptContrAlg.cpp
	./source/TasksApi.cpp
)

set(SOURCE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../release/libMathPlot.so)
add_executable(CoreServer ${SOURCE_EXE})


target_link_libraries(CoreServer PRIVATE 
    ${BOOST_URL_LIB}
    ${BOOST_COROUTINE_LIB}
    ${BOOST_CONTEXT_LIB}
  pthread
  dl
	${SOURCE_LIB}
)
target_include_directories(CoreServer PRIVATE
    ./include/
    ${BOOST_INCLUDE_DIR}
)
