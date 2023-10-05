//
// Created by pepos on 10/3/2023.
//

#ifndef WINSTESTS_SOCKET_H
#define WINSTESTS_SOCKET_H
#include "SocketHandle.h"
#include "tresults.h"
#include "ipversion.h"
#include "socketOptions.h"
#include "IPEndPoint.h"
#pragma once
namespace Tnet{
    class Socket
            {
    public:
        Socket(IPVersion ipversion = IPVersion::IPv4,
               SocketHandle handle = INVALID_SOCKET);
        TResults Create();
        TResults Close();
        TResults Bind(IPEndpoint endPoint);
        TResults Listen(IPEndpoint endPoint,int backlog=5);
        TResults Accept(Socket &socket);
        TResults Connect(IPEndpoint endPoint);
        TResults Send(void * data, size_t size, int &bytesSent);
        TResults Recv(void * data, size_t size, int &bytesReceived);
        TResults SendAll(void * data, size_t size);
        TResults RecvAll(void * data, size_t size);
        SocketHandle GetHandle();
        IPVersion GetIPversion();
    private:
        TResults SetSocketOptions(SocketOptions options, BOOL value);
        IPVersion ipversion = IPVersion::IPv4;
        SocketHandle handle = INVALID_SOCKET;
    };
}
#endif //WINSTESTS_SOCKET_H
