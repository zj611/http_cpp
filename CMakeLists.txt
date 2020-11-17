cmake_minimum_required(VERSION 3.15)
project(httplib)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(include/http)
add_subdirectory(include/downloadModule)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/CJSON)
include_directories(include)

add_executable(http_test
        main.cpp
        )
target_link_libraries(http_test
        -pthread
        http
        download
        CJSON
        )

