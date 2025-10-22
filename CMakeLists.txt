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

    set(ASIO_ZIP "${PROJECT_SOURCE_DIR}/include/asio.zip")
    set(ASIO_DIR "${PROJECT_SOURCE_DIR}/include/asio-master")

    file(DOWNLOAD
        https://github.com/chriskohlhoff/asio/archive/refs/heads/master.zip
        ${ASIO_ZIP}
        SHOW_PROGRESS
    )

    # Extract the zip file (uses cmake -E tar to unzip)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${ASIO_ZIP}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/include
        RESULT_VARIABLE unzip_result
    )

    # If unzip failed, try using 'unzip' command as fallback
    if(NOT unzip_result EQUAL 0)
        message(WARNING "CMake unzip failed, trying system unzip...")
        execute_process(
            COMMAND unzip -o ${ASIO_ZIP} -d ${PROJECT_SOURCE_DIR}/include
        )
    endif()

    # Move the extracted files into place
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASIO_DIR}/asio
        ${PROJECT_SOURCE_DIR}/include/asio
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy
        ${ASIO_DIR}/asio.hpp
        ${PROJECT_SOURCE_DIR}/include/asio.hpp
    )

    # Clean up
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ASIO_DIR}
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E remove ${ASIO_ZIP}
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
