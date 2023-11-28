cmake_minimum_required(VERSION 3.26)
project(winstests)

set(CMAKE_CXX_STANDARD 17)

# Add the Tnet library
add_library(Tnet STATIC
        Tnet/network.cpp
        Tnet/network.h
        Tnet/tresults.h
        Tnet/ipversion.h
        Tnet/Socket.cpp
        Tnet/Socket.h
        Tnet/SocketHandle.h
        Tnet/IPEndPoint.h
        Tnet/IPEndPoint.cpp
        Tnet/Constants.h
        Tnet/Packet.h
        Tnet/PacketException.h
)

# Link the ws2_32 library
target_link_libraries(Tnet PRIVATE ws2_32)

# Include the Tnet directory to access Tnet.h
target_include_directories(Tnet PUBLIC ${CMAKE_SOURCE_DIR}/Tnet)

# Create separate executable targets for client and server
add_executable(client_exe
        client/client.cpp
        Tnet/Packet.h
        Tnet/Packet.cpp
        Tnet/PacketException.h
)

add_executable(server_exe
        server/server.cpp
        Tnet/Packet.h
        Tnet/Packet.cpp
        Tnet/PacketException.h
)

# Link the client_exe and server_exe with the Tnet library
target_link_libraries(client_exe PRIVATE Tnet)
target_link_libraries(server_exe PRIVATE Tnet)
