cmake_minimum_required(VERSION 3.29)
project(califoria-king)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VENDOR ${CMAKE_SOURCE_DIR}/vendor)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${VENDOR}/SFML/include)

if(EXISTS ${VENDOR}/SFML/CMakeLists.txt)
    add_subdirectory(${VENDOR}/SFML)
else()
    message(FATAL_ERROR "SFML not found in the vendor directory!")
endif()

add_executable(main main.cpp)

target_link_libraries(main sfml-graphics sfml-window sfml-system sfml-audio)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    target_link_libraries(main "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
    target_compile_options(main PRIVATE "-stdlib=libc++")
endif()
