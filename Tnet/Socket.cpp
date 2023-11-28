//
// Created by pepos on 10/3/2023.
//
#include "SocketHandle.h"
#include "tresults.h"
#include "ipversion.h"
#include "Socket.h"
#include "Packet.h"
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
                return TResults::T_GenericError;
            }
            handle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//attempt to create socket
            if(handle == INVALID_SOCKET){
                int error = WSAGetLastError();
                return TResults::T_GenericError;
            }

            if (SetSocketOptions(SocketOptions::TCP_NoDelay, TRUE) != TResults::T_Success){
                return TResults::T_GenericError;
            }
            return TResults::T_Success;

        }

        TResults Socket::Close() {
            if(handle == INVALID_SOCKET){
                return TResults::T_GenericError;
            }
            int result = closesocket(handle);
            if (result != 0){
                int error = WSAGetLastError();
                return TResults::T_GenericError;
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
                return TResults::T_GenericError;
        }

        if (result != 0){ //error occured
            int error = WSAGetLastError();
            return TResults::T_GenericError;
        }
        return TResults::T_Success;

    }

    TResults Socket::Bind(IPEndpoint endPoint) {
            sockaddr_in addr = endPoint.GetSockAddr();
            int result = bind(handle, (sockaddr *)(&addr), sizeof(sockaddr_in));
        if (result != 0){
            int error = WSAGetLastError();
            return TResults::T_GenericError;
        }
        return TResults::T_Success;
    }

    TResults Socket::Listen(IPEndpoint endPoint, int backlog) {
            if (Bind(endPoint) != TResults::T_Success){
                return TResults::T_GenericError;
            }
            int result = listen(handle, backlog);
            if (result != 0){
                int error = WSAGetLastError();
                return TResults::T_GenericError;
            }
            return TResults::T_Success;
    }

    TResults Socket::Accept(Socket &socket) {
            sockaddr_in addr = {};
            int len = sizeof(sockaddr_in);
            SocketHandle acceptConnectionHandle = accept(handle, (sockaddr *)&addr, &len);
            if (acceptConnectionHandle == INVALID_SOCKET){
                int error = WSAGetLastError();
                return TResults::T_GenericError;
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
            return TResults::T_GenericError;
        }
        return TResults::T_Success;
    }

    TResults Socket::Send(const void *data, uint32_t size, int &bytesSent) {
            bytesSent = send(handle, (const char *)data, size, 0);
        if (bytesSent == SOCKET_ERROR){
            int error = WSAGetLastError();
            return TResults::T_GenericError;
        }
        return TResults::T_Success;
    }

    TResults Socket::Recv(void * destination, uint32_t numberOfBytes, int & bytesReceived)
    {
        bytesReceived = recv(handle, (char*)destination, numberOfBytes, NULL);

        if (bytesReceived == 0) //If connection was gracefully closed
        {
            return TResults::T_GenericError;
        }

        if (bytesReceived == SOCKET_ERROR)
        {
            int error = WSAGetLastError();
            return TResults::T_GenericError;
        }

        return TResults::T_Success;
    }

    TResults Socket::SendAll(const void *data, uint32_t size) {
        int totalBytesSent = 0;
        while (totalBytesSent < size){
            int bytesRemaining = size - totalBytesSent;
            char *dataRemaining = (char *)data + totalBytesSent;
            int bytesSent = send(handle, (const char *)data + totalBytesSent, size - totalBytesSent, 0);
            TResults result = Send(dataRemaining, bytesRemaining, bytesSent);
            if (result != TResults::T_Success){
                return TResults::T_GenericError;
            }
            totalBytesSent += bytesSent;
        }
        return TResults::T_Success;
    };
    TResults Socket::RecvAll(void * destination, uint32_t numberOfBytes)
    {
        int totalBytesReceived = 0;

        while (totalBytesReceived < numberOfBytes)
        {
            int bytesRemaining = numberOfBytes - totalBytesReceived;
            int bytesReceived = 0;
            char * bufferOffset = (char*)destination + totalBytesReceived;
            TResults result = Recv(bufferOffset, bytesRemaining, bytesReceived);
            if (result != TResults::T_Success)
            {
                return TResults::T_GenericError;
            }
            totalBytesReceived += bytesReceived;
        }

        return TResults::T_Success;
    }

    TResults Socket::Send(Packet &packet) {
        uint32_t encodedPacketSize = htonl(packet.buffer.size());
        TResults results = SendAll(&encodedPacketSize, sizeof(uint32_t));
        if (results != TResults::T_Success){
            return TResults::T_GenericError;
        }
        results = SendAll(packet.buffer.data(), packet.buffer.size());
        if (results != TResults::T_Success){
            return TResults::T_GenericError;
        }
        return TResults::T_Success;
    }

    TResults Socket::Recv(Packet & packet)
    {
        packet.Clear();

        size_t encodedSize = 0;
        TResults result = RecvAll(&encodedSize, sizeof(uint32_t)); //Recv packet size
        if (result != TResults::T_Success)
            return TResults::T_GenericError;

        uint32_t bufferSize = ntohl(encodedSize);

        packet.buffer.resize(bufferSize);
        result = RecvAll(&packet.buffer[0], packet.buffer.size()); //Recv packet data
        if (result != TResults::T_Success)
            return TResults::T_GenericError;

        return TResults::T_Success;
    }

}
