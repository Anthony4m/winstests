//
// Created by pepos on 10/3/2023.
//
#include "SocketHandle.h"
#include "tresults.h"
#include "ipversion.h"
#include "Socket.h"
#include <cassert>
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

}
