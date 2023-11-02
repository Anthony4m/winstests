#include <iostream>
#include "../Tnet/Tnet.h"
using namespace Tnet;
int main() {
    if (Network::Initialize()) {
        std::cout << "Initialized" << std::endl;
        //server to bind port 1996

        Socket socket;
        if (socket.Create() == TResults::T_Success) {
            std::cout << "Socket Successfully created" << std::endl;
            // server to listen for connections on 1995
            if (socket.Listen(IPEndpoint("0.0.0.0", 1996)) == TResults::T_Success) {
                std::cout << "Socket Successfully Listening to 1996" << std::endl;
                Socket clientConnection;
                if (socket.Accept(clientConnection) == TResults::T_Success) {
                    std::cout << "Socket Successfully Connected" << std::endl;
                    uint32_t a, b, c;
                    Packet packet;
                    packet << a << b << c;
                    while (true) {
                        TResults result = clientConnection.Recv(packet);
                        if (result != TResults::T_Success) {
                            std::cout << "Recv failed" << std::endl;
                            break;
                        }
                        try{
                        packet >> a >> b >> c;
                        std::cout << "Received: " << a << " " << b << " " << c << std::endl;
                        }catch(PacketException &exception){
                            std::cout << exception.what() << std::endl;
                            std::cout << exception.ToString() << std::endl;

                        }
                    }
                }

                socket.Close();
            } else {
                std::cout << "Socket failed to  create" << std::endl;
            }
        }
        Network::Shutdown();
        system("pause");

        return 0;
    }
}
