//
// Created by gabzdev on 12.03.2025.
//
#pragma once

#include <windows.h>
#include <wlanapi.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

#ifndef WIFI_WINDOWS_H
#define WIFI_WINDOWS_H

#define MAX_PEERS 1000

DWORD openWlanHandle(PHANDLE phClientHandle, PDWORD pdwNegotiatedVersion);
DWORD getWlanInterfaces(HANDLE hClientHandle, PWLAN_INTERFACE_INFO_LIST* ppInterfaceList);
DWORD configureHostedNetwork(HANDLE hClientHandle, const char* ssid, const char* password);
DWORD startHostedNetwork(HANDLE hClientHandle);
DWORD stopHostedNetwork(HANDLE hClientHandle);
void printWlanInterfaces(PWLAN_INTERFACE_INFO_LIST pInterfaceList);

#endif //WIFI_WINDOWS_H
