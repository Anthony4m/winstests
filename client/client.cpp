#include <iostream>
#include "../Tnet/Tnet.h"
using namespace Tnet;
int main() {
    if (Network::Initialize()) {
        std::cout << "Initialized" << std::endl;
        Socket socket;
        if (socket.Create() == TResults::T_Success) {
            std::cout << "Socket Successfully created" << std::endl;
            // connect to server on 1995
            if (socket.Connect(IPEndpoint("localhost", 1996)) == TResults::T_Success) {
                std::cout << "Socket Successfully Connected" << std::endl;
                uint32_t a, b, c;
                a = 4;
                b = 7;
                c = 9;
                Packet packet;
                packet << a << b << c;
                while (true) {
                    TResults result = socket.Send(packet);
                    if (result != TResults::T_Success) {
                        std::cout << "Send failed" << std::endl;
                        break;
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