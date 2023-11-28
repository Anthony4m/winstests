//
// Created by pepos on 10/24/2023.
//
#include <winsock2.h>
#include <vector>
#include <cstdint>
#include <string>
#include "PacketException.h"

#ifndef WINSTESTS_PACKET_H
#define WINSTESTS_PACKET_H
namespace Tnet {
    class Packet {
    public:
        void Clear();
        void Append(const void * data, uint32_t size);
        Packet& operator<<(uint32_t data);
        Packet& operator>>(uint32_t & data);
        Packet& operator<<(const std::string & data);
        Packet& operator>>(std::string & data);

        size_t extractionOffset = 0;
        std::vector<unsigned char> buffer;
    };
}
#endif //WINSTESTS_PACKET_H
