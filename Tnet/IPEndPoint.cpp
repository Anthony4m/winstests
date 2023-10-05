//
// Created by pepos on 10/3/2023.
//
#include "IPEndPoint.h"
#include <WS2tcpip.h>
#include <cassert>
#include <iostream>

Tnet::IPEndpoint::IPEndpoint(const char *ip, unsigned short port) {
    this->port = port;

    in_addr addr; //location to store the ipv4 address
    int result = inet_pton(AF_INET, ip, &addr); //convert the string to ipv4 address
    if (result == 1) {
        if (addr.S_un.S_addr != INADDR_NONE) {
            hostname = ip;
            Ipaddress = ip;
            Ipbytes.resize(sizeof (ULONG));
            memcpy(&Ipbytes[0], &addr.S_un.S_addr, sizeof (ULONG));
            ipversion = IPVersion::IPv4;
            return;
        }
    }
    //Attempt to resolve hostname to ipv4 address
    addrinfo hints = {}; //hints will filter results we get back for getaddrinfo
    hints.ai_family =  AF_INET; //ipv4 address only
    addrinfo *hostinfo = nullptr;
    result = getaddrinfo(ip,NULL,&hints,&hostinfo); //get the address info
    if (result == 0) {
        sockaddr_in * host_addr = reinterpret_cast<sockaddr_in *>(hostinfo->ai_addr);
        Ipaddress.resize(16);
        inet_ntop(AF_INET,&host_addr->sin_addr,&Ipaddress[0],16);
        hostname = ip;
        ULONG ip_long = host_addr->sin_addr.S_un.S_addr;
        Ipbytes.resize(sizeof (ULONG));
        memcpy(&Ipbytes[0], &ip_long, sizeof (ULONG));
        ipversion = IPVersion::IPv4;
        freeaddrinfo(hostinfo);
        return;
    }
}

Tnet::IPVersion Tnet::IPEndpoint::GetIPversion() {
    return ipversion;
}

std::string Tnet::IPEndpoint::GetHostname() {
    return hostname;
}

std::string Tnet::IPEndpoint::GetIPAddress() {
    return Ipaddress;
}

std::vector<uint8_t> Tnet::IPEndpoint::GetIPBytes() {
    return Ipbytes;
}

unsigned short Tnet::IPEndpoint::GetPort() {
    return port;
}

sockaddr_in Tnet::IPEndpoint::GetSockAddr() {
    assert(ipversion == IPVersion::IPv4);
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr, &Ipbytes[0], sizeof (ULONG));
    addr.sin_port = htons(port);
    return addr;
}

Tnet::IPEndpoint::IPEndpoint(sockaddr *host_addr) {
    assert(host_addr->sa_family == AF_INET);
    sockaddr_in *host_addr_in = reinterpret_cast<sockaddr_in *>(host_addr);
    Ipaddress.resize(16);
    port = ntohs(host_addr_in->sin_port);
    ipversion = IPVersion::IPv4;
    inet_ntop(AF_INET,&host_addr_in->sin_addr,&Ipaddress[0],16);
    hostname = Ipaddress;
    ULONG ip_long = host_addr_in->sin_addr.S_un.S_addr;
    Ipbytes.resize(sizeof (ULONG));
    memcpy(&Ipbytes[0], &ip_long, sizeof (ULONG));

}

void Tnet::IPEndpoint::Print() {
    switch (ipversion) {
        case IPVersion::IPv4:
            std::cout << "IPv4: " << hostname << ":" << port << std::endl;
            break;
        case IPVersion::IPv6:
            std::cout << "IPv6: " << hostname << ":" << port << std::endl;
            break;
        default:
            std::cout << "Unknown IP version" << std::endl;
            break;
    }
    std::cout << "Hostname: " << hostname << std::endl;
    std::cout << "IP address: " << Ipaddress << std::endl;
    for (auto & i : Ipbytes) {
        std::cout << (int)i << std::endl;

    }
    std::cout << "Port: " << port << std::endl;

}
