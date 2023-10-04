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
        SocketHandle GetHandle();
        IPVersion GetIPversion();
    private:
        TResults SetSocketOptions(SocketOptions options, BOOL value);
        IPVersion ipversion = IPVersion::IPv4;
        SocketHandle handle = INVALID_SOCKET;
    };
}
#endif //WINSTESTS_SOCKET_H
