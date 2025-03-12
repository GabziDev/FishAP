cmake_minimum_required(VERSION 3.11)
project(FishAP C)

set(CMAKE_C_STANDARD 11)

set(SOURCES
        src/main.c
        src/logger.c
        src/include/logger.h
)

if(WIN32)
    list(APPEND SOURCES
            src/wifi_windows.c
            src/include/wifi_windows.h
    )
elseif(UNIX)
    list(APPEND SOURCES
            src/wifi_linux.c
            src/include/wifi_linux.h
    )
endif()

add_executable(FishAP ${SOURCES})


