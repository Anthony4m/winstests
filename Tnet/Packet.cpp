//
// Created by pepos on 10/24/2023.
//
#include "Packet.h"
namespace Tnet {
    void Packet::Clear() {
        buffer.clear();
        extractionOffset = 0;
    }

    void Packet::Append(const void * data, uint32_t size)
    {
        if(extractionOffset + size > buffer.size()){
            throw PacketException("[Packet::Append(Const void *, uint32_t)] - Packet size Max Packet Size");
        }
        buffer.insert(buffer.end(), (unsigned char*)data, (unsigned char*)data + size);
    }

    Packet & Packet::operator<<(uint32_t data)
    {
        data = htonl(data);
        Append(&data, sizeof(uint32_t));
        return *this;
    }
    Packet & Packet::operator>>(uint32_t & data)
    {
        if (extractionOffset + sizeof(uint32_t) > buffer.size()){
            throw PacketException("[Packet::operator>>(uint32_t &)] - Extraction Offset + sizeof(uint32_t) > buffer.size()");
        }
        data = *reinterpret_cast<const uint32_t*>(&buffer[extractionOffset]);
        extractionOffset += sizeof(uint32_t);
        data = ntohl(data);
        return *this;
    }

    Packet &Packet::operator<<(const std::string &data) {
        *this << (size_t)data.size();
        Append(data.data(), data.size());
        return *this;
    }

    Packet &Packet::operator>>(std::string &data) {
        data.clear();
        size_t size = 0;
        *this >> reinterpret_cast<std::string &>(size);
        if (extractionOffset + sizeof(uint32_t) > buffer.size()){
            throw PacketException("[Packet::operator>>(uint32_t &)] - Extraction Offset + sizeof(uint32_t) > buffer.size()");
        }
        data.resize(size);
        data.assign(reinterpret_cast<const char *>(&buffer[extractionOffset]), size);
        extractionOffset += size;
        return *this;
    }
}