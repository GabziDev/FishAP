//
// Created by gabzdev on 12.03.2025.
//
#pragma once

#ifdef _WIN32
    #include "wifi_windows.h"
#elif __linux__
    #include "wifi_linux.h"
#else
    #error "Platform not supported"
#endif

#ifndef WIFI_H
#define WIFI_H

#endif //WIFI_H
