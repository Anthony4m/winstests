//
// Created by pepos on 10/3/2023.
//
#include "SocketHandle.h"
#include "tresults.h"
#include "ipversion.h"
#include "Socket.h"
#include <cassert>
#include <iostream>

namespace Tnet{
        Socket::Socket(IPVersion ipversion , SocketHandle handle )
        :ipversion(ipversion),handle(handle) {
            assert(ipversion == IPVersion::IPv4);
        }

        TResults Socket::Create() {
            assert(ipversion == IPVersion::IPv4);
            if (handle != INVALID_SOCKET){
                return TResults::T_NotYetImplemented;
            }
            handle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//attempt to create socket
            if(handle == INVALID_SOCKET){
                int error = WSAGetLastError();
                return TResults::T_NotYetImplemented;
            }

            if (SetSocketOptions(SocketOptions::TCP_NoDelay, TRUE) != TResults::T_Success){
                return TResults::T_NotYetImplemented;
            }
            return TResults::T_Success;

        }

        TResults Socket::Close() {
            if(handle == INVALID_SOCKET){
                return TResults::T_NotYetImplemented;
            }
            int result = closesocket(handle);
            if (result != 0){
                int error = WSAGetLastError();
                return TResults::T_NotYetImplemented;
            }
            handle = INVALID_SOCKET;
            return TResults::T_Success;
        }

        SocketHandle Socket::GetHandle() {
            return handle;
        }

        IPVersion Socket::GetIPversion() {
            return ipversion;
        }

    TResults Socket::SetSocketOptions(SocketOptions options, BOOL value) {
            int result = 0;
        switch (options) {
            case SocketOptions::TCP_NoDelay:
                result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *)&value, sizeof(value));
                break;
            default:
                return TResults::T_NotYetImplemented;
        }

        if (result != 0){ //error occured
            int error = WSAGetLastError();
            return TResults::T_NotYetImplemented;
        }
        return TResults::T_Success;

    }

    TResults Socket::Bind(IPEndpoint endPoint) {
            sockaddr_in addr = endPoint.GetSockAddr();
            int result = bind(handle, (sockaddr *)(&addr), sizeof(sockaddr_in));
        if (result != 0){
            int error = WSAGetLastError();
            return TResults::T_NotYetImplemented;
        }
        return TResults::T_Success;
    }

    TResults Socket::Listen(IPEndpoint endPoint, int backlog) {
            if (Bind(endPoint) != TResults::T_Success){
                return TResults::T_NotYetImplemented;
            }
            int result = listen(handle, backlog);
            if (result != 0){
                int error = WSAGetLastError();
                return TResults::T_NotYetImplemented;
            }
            return TResults::T_Success;
    }

    TResults Socket::Accept(Socket &socket) {
            sockaddr_in addr = {};
            int len = sizeof(sockaddr_in);
            SocketHandle acceptConnectionHandle = accept(handle, (sockaddr *)&addr, &len);
            if (acceptConnectionHandle == INVALID_SOCKET){
                int error = WSAGetLastError();
                return TResults::T_NotYetImplemented;
            }
            IPEndpoint connectionEndPoint((sockaddr*)&addr);
            std::cout << "Connection from " << connectionEndPoint.GetIPAddress() << ":" << connectionEndPoint.GetPort() << std::endl;
            connectionEndPoint.Print();
            socket = Socket(IPVersion::IPv4, acceptConnectionHandle);
        return TResults::T_Success;
    }

    TResults Socket::Connect(IPEndpoint endPoint) {
            sockaddr_in addr = endPoint.GetSockAddr();
            int result = connect(handle, (sockaddr *)(&addr), sizeof(sockaddr_in));
        if (result != 0){
            int error = WSAGetLastError();
            return TResults::T_NotYetImplemented;
        }
        return TResults::T_Success;
    }

    TResults Socket::Send(void *data, size_t size, int &bytesSent) {
            bytesSent = send(handle, (const char *)data, size, 0);
        if (bytesSent == SOCKET_ERROR){
            int error = WSAGetLastError();
            return TResults::T_NotYetImplemented;
        }
        return TResults::T_Success;
    }

    TResults Socket::Recv(void *data, size_t size, int &bytesReceived) {
            bytesReceived = recv(handle, (char *)data, size, 0);
            if (bytesReceived == 0){
                return TResults::T_NotYetImplemented;
            }
        if (bytesReceived == SOCKET_ERROR){
            int error = WSAGetLastError();
            return TResults::T_NotYetImplemented;
        }
        return TResults::T_Success;
    }

    TResults Socket::SendAll(void *data, size_t size) {
        int totalBytesSent = 0;
        while (totalBytesSent < size){
            int bytesRemaining = size - totalBytesSent;
            char *dataRemaining = (char *)data + totalBytesSent;
            int bytesSent = send(handle, (const char *)data + totalBytesSent, size - totalBytesSent, 0);
            TResults result = Send(dataRemaining, bytesRemaining, bytesSent);
            if (result != TResults::T_Success){
                return TResults::T_NotYetImplemented;
            }
            totalBytesSent += bytesSent;
        }
        return TResults::T_Success;
    };
    TResults Socket::RecvAll(void *data, size_t size) {
        int totalBytesReceived = 0;
        while (totalBytesReceived < size){
            int bytesRemaining = size - totalBytesReceived;
            char *dataRemaining = (char *)data + totalBytesReceived;
            int bytesSent = recv(handle, ( char *)data + totalBytesReceived, size - totalBytesReceived, 0);
            TResults result = Send(dataRemaining, bytesRemaining, bytesSent);
            if (result != TResults::T_Success){
                return TResults::T_NotYetImplemented;
            }
            totalBytesReceived += bytesSent;
        }
        return TResults::T_Success;
    };

}
