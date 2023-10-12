//
// Created by pepos on 10/3/2023.
//
#include "ipversion.h"
#include <string>
#include <vector>
#include <cstdint>
#include <WS2tcpip.h>

#ifndef WINSTESTS_IPENDPOINT_H
#define WINSTESTS_IPENDPOINT_H
namespace Tnet {
    class IPEndpoint {
    public:
        IPEndpoint(const char * ipaddress, unsigned short port);
        IPEndpoint(sockaddr * host_addr);
        IPVersion GetIPversion();
        std::string GetHostname();
        std::string GetIPAddress();
        std::vector<uint8_t> GetIPBytes();
        unsigned short GetPort();
        sockaddr_in GetSockAddr();
        void Print();
    private:
         IPVersion ipversion = IPVersion::Unknown;
         std::string hostname;
         std::string Ipaddress;
         std::vector<uint8_t> Ipbytes;
         unsigned short port=0;
    };
}
#endif //WINSTESTS_IPENDPOINT_H
