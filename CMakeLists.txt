cmake_minimum_required(VERSION 3.27)

project(exa-cpp
    VERSION 0.1.0
    DESCRIPTION "low-latency C++20 client for Exa's search API"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter
        -fno-rtti
    )
endif()

if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_compile_options(-O3 -DNDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_options(-O0 -g -fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

# add library target for client
add_library(exa_client src/client.cpp)
target_include_directories(exa_client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(exa_client
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_executable(poc src/poc.cpp)

target_link_libraries(poc 
    exa_client
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_include_directories(poc PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)