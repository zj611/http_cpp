set(CJSON
        CJsonObject.cpp
        cJSON.c
)

add_library(CJSON STATIC  #STATIC SHARED 必须大写
        ${CJSON})

SET (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)