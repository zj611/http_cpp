set(download
    HttpClient.cpp
)
# sudo apt-get install -y openssl libssl-dev libcurl4-openssl-dev
find_package(CURL)
link_libraries(curl)

add_library(download SHARED  #STATIC SHARED 必须大写
        ${download})

SET (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)