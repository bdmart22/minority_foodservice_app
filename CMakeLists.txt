cmake_minimum_required(VERSION 3.10)
project(foodapp-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add dependencies automatically
include(ExternalProject)
include(FetchContent)

# Create include dir if missing
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)

# --- Download Crow if missing ---
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/include/crow_all.h")
    message(STATUS "Downloading Crow header...")
    file(DOWNLOAD
        https://github.com/CrowCpp/Crow/releases/latest/download/crow_all.h
        ${PROJECT_SOURCE_DIR}/include/crow_all.h
        SHOW_PROGRESS
    )
endif()

# --- Download standalone Asio if missing ---
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/include/asio.hpp")
    message(STATUS "Downloading Asio (standalone)...")
    file(DOWNLOAD
        https://github.com/chriskohlhoff/asio/archive/refs/heads/master.zip
        ${PROJECT_SOURCE_DIR}/include/asio.zip
        SHOW_PROGRESS
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_SOURCE_DIR}/include/asio.zip
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/include
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/include/asio-master/asio
        ${PROJECT_SOURCE_DIR}/include/asio
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/include/asio-master/asio.hpp
        ${PROJECT_SOURCE_DIR}/include/asio.hpp
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/include/asio-master
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_SOURCE_DIR}/include/asio.zip
    )
endif()

# Source files
add_executable(foodapp-server
    main.cpp
    src/db.cpp
    src/translate.cpp
)

# Libraries
target_link_libraries(foodapp-server
    pthread
    curl
    mysqlclient
)
