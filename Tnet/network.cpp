//
// Created by pepos on 10/3/2023.
//
#include <iostream>
#include "network.h"

    bool Tnet::Network::Initialize() {
        WSADATA wsaData;
        int iResult;
    // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            std::cerr << "WSAStartup failed: " << iResult<<std::endl;
            return false;
        }
        return true;
    };

    void Tnet::Network::Shutdown() {
        WSACleanup();
    };
