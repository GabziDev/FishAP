#include <stdio.h>

#include "include/wifi.h"

void test() {
#ifdef _WIN32
    DWORD dwError = 0;
    HANDLE hClientHandle = NULL;
    DWORD dwNegotiatedVersion = 0;

    if (openWlanHandle(&hClientHandle, &dwNegotiatedVersion) != ERROR_SUCCESS) {
        printf("Failed to open Wlan interface\n");
        return;
    }

    const char* ssid = "wifidetest";
    const char* password = "123456789$$$$";

    dwError = configureHostedNetwork(hClientHandle, ssid, password);
    if (dwError != ERROR_SUCCESS) {
        WlanCloseHandle(hClientHandle, NULL);
        return;
    }

    dwError = startHostedNetwork(hClientHandle);
    if (dwError != ERROR_SUCCESS) {
        WlanCloseHandle(hClientHandle, NULL);
        return;
    }

    printf("AP Start (SSID->%s - PWD->%s)\n", ssid, password);

    getchar();

    stopHostedNetwork(hClientHandle);

    WlanCloseHandle(hClientHandle, NULL);

    printf("test completed _WIN32\n");
#endif
#ifdef __linux__
    printf("test completed __linux__\n");
#endif
}

int main(void) {
    test();

    return 0;
}