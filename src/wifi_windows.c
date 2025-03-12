//
// Created by gabzdev on 12.03.2025.
//

#include "include/wifi_windows.h"

#include <stdio.h>

/*
 * Ouvrir une reference de l'API Wlan
 *
 * Param:
 * - phClientHandle : pointeur de la reference de l'API Wlan
 * - pdwNegotiatedVersion : pointeur vers la version negociee de l'API Wlan
 *
 * Return:
 *  Retourne le code de resultat de l'appel à la fonction WlanOpenHandle
 */
DWORD openWlanHandle(PHANDLE phClientHandle, PDWORD pdwNegotiatedVersion) {
    return WlanOpenHandle(2, NULL, pdwNegotiatedVersion, phClientHandle);
}

/*
 * Recuperer les interface Wlan
 *
 * Param:
 * - hClientHandle : reference du client WLAN pour acceder a l'API Wlan
 * - ppInterfaceList : pointeur vers un pointeur qui recevra la list des interfaces Wlan
 *
 * Return:
 *  Retourne le code de resultat de l'appel à la fonction WlanEnumInterfaces
 */
DWORD getWlanInterfaces(HANDLE hClientHandle, PWLAN_INTERFACE_INFO_LIST* ppInterfaceList) {
    return WlanEnumInterfaces(hClientHandle, NULL, ppInterfaceList);
}

/*
 * Configure le reseau wifi
 *
 * Param:
 * - hClientHandle : reference du client WLAN pour acceder a l'API Wlan
 * - ssid : pointeur du nom du wifi
 * - password : pointeur du mot de passe du wifi
 *
 * Return:
 *  Retourne un code d'erreur
 */
DWORD configureHostedNetwork(HANDLE hClientHandle, const char* ssid, const char* password) {
    DWORD dwError = 0;
    WLAN_HOSTED_NETWORK_REASON reason = {0};
    WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS connectionSettings = {0};
    WLAN_INTERFACE_INFO_LIST* pInterfaceInfoList = NULL;

    dwError = getWlanInterfaces(hClientHandle, &pInterfaceInfoList);
    if (dwError != ERROR_SUCCESS) {
        printf("Erreur lors de la recuperation des interfaces %d\n", dwError);
        return dwError;
    }

    if (pInterfaceInfoList->dwNumberOfItems == 0) {
        printf("Aucune interface Wifi disponible\n");
        WlanFreeMemory(pInterfaceInfoList);
        return ERROR_NO_MORE_DEVICES;
    }

    for (int i = 0; i < pInterfaceInfoList->dwNumberOfItems; i++) {
        PWLAN_INTERFACE_INFO pInterfaceInfo = &pInterfaceInfoList->InterfaceInfo[i];
        printf("Interface %d -> %ws\n", i + 1, pInterfaceInfo->strInterfaceDescription);
        if (pInterfaceInfo->isState != wlan_interface_state_connected) {
            printf("L'interface %d n'est pas connectee\n", i + 1);
            continue;
        }
    }

    BOOL enable = TRUE;
    dwError = WlanHostedNetworkSetProperty(hClientHandle,
                                           wlan_hosted_network_opcode_enable,
                                           sizeof(BOOL),
                                           &enable,
                                           &reason,
                                           NULL);
    if (dwError != ERROR_SUCCESS) {
        printf("Erreur lors de l'activation du reseau heberge: %lu (%d)\n", dwError, reason);
        WlanFreeMemory(pInterfaceInfoList);
        return dwError;
    }

    // Config ssid
    connectionSettings.dwMaxNumberOfPeers = MAX_PEERS; // Nombre de clients max
    size_t ssidLen = strlen(ssid);
    connectionSettings.hostedNetworkSSID.uSSIDLength = (UCHAR)ssidLen; // Taille du ssid
    memcpy(connectionSettings.hostedNetworkSSID.ucSSID, ssid, ssidLen); // copier le ssid dans les parametres

    //    printf("[DEBUG] %s:%s\n", ssid, password);

    dwError = WlanHostedNetworkSetProperty(hClientHandle,
        wlan_hosted_network_opcode_connection_settings,
        sizeof(connectionSettings),
        &connectionSettings,
        &reason, NULL);
    if (dwError != ERROR_SUCCESS) {
        printf("Erreur lors de la configuration du ssid: %lu (%d)", reason,  dwError);
        WlanFreeMemory(pInterfaceInfoList);
        return dwError;
    }

    dwError = WlanHostedNetworkSetSecondaryKey(hClientHandle,
        strlen(password) + 1,
        (PUCHAR)password,
        TRUE,
        TRUE,
        &reason, NULL);
    if (dwError != ERROR_SUCCESS) {
        printf("Erreur lors de la configuration du ssid: %lu (%d)", reason,  dwError);
        WlanFreeMemory(pInterfaceInfoList);
        return dwError;
    }

    WlanFreeMemory(pInterfaceInfoList);

    return ERROR_SUCCESS;
}

DWORD startHostedNetwork(HANDLE hClientHandle) {
    DWORD dwError = 0;
    WLAN_HOSTED_NETWORK_REASON reason = {0};

    dwError = WlanHostedNetworkForceStart(hClientHandle, &reason, NULL);
    if (dwError != ERROR_SUCCESS) {
        printf("Erreur lors du demarrage du reseau wifi: %lu (%d)", reason, dwError);
    }

    return dwError;
}

DWORD stopHostedNetwork(HANDLE hClientHandle) {
    DWORD dwError = 0;
    WLAN_HOSTED_NETWORK_REASON reason = {0};

    dwError = WlanHostedNetworkStopUsing(hClientHandle, &reason, NULL);
    if (dwError != ERROR_SUCCESS) {
        printf("Erreur lors de l'arret du reseau wifi: %lu (%d)", reason, dwError);
    }

    WlanCloseHandle(hClientHandle, NULL);

    return dwError;
}

/*
 * Afficher dans la console les interfaces Wlan
 *
 * Param:
 * - pInterfaceList : liste des interfaces Wlan
 *
 * Return:
 *  Retourne rien
 */
void printWlanInterfaces(PWLAN_INTERFACE_INFO_LIST pInterfaceList) {
    if (pInterfaceList->dwNumberOfItems == 0) {
        printf("Aucune interface Wifi disponible\n");
        return;
    }

    for (int i = 0; i < pInterfaceList->dwNumberOfItems; i++) {
        PWLAN_INTERFACE_INFO pInterfaceInfo = &pInterfaceList->InterfaceInfo[i];
        printf("Interface %d -> %ws\n", i + 1, pInterfaceInfo->strInterfaceDescription);
    }

    WlanFreeMemory(pInterfaceList);
}