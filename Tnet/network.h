//
// Created by pepos on 10/3/2023.
//

#ifndef WINSTESTS_NETWORK_H
#define WINSTESTS_NETWORK_H
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#define WIN32_LEAN_AND_MEAN
namespace Tnet {
    class Network {
    public:
        static bool Initialize();
        static void Shutdown();
    };
}
#endif //WINSTESTS_NETWORK_H
