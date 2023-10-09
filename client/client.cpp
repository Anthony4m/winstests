#include <iostream>
#include "../Tnet/Tnet.h"
using namespace Tnet;
int main() {
    if(Network::Initialize()) {
        std::cout << "Initialized" << std::endl;
        Socket socket;
        if (socket.Create() == TResults::T_Success){
            std::cout<<"Socket Successfully created" <<std::endl;
            // connect to server on 1995
            if (socket.Connect(IPEndpoint("localhost",1996)) == TResults::T_Success){
                std::cout<<"Socket Successfully Connected" <<std::endl;
                std::string buffer = "Hello from client";
               while (true){
                    uint32_t bufferSize = buffer.size();
                    bufferSize = htonl(bufferSize);
                    auto result = socket.SendAll(buffer.data(),sizeof(uint32_t));
                    if (result != TResults::T_Success){
                        std::cout<<"Socket failed to send" <<std::endl;
                        break;
                    }
                    result = socket.SendAll(buffer.data(),buffer.size());
                    if (result != TResults::T_Success){
                        std::cout<<"Socket failed to send" <<std::endl;
                        break;
                    }
                    std::cout<<"Sending Data.....: "<<"["<<bufferSize<<"]"<<buffer<<std::endl;
                    Sleep(500);
                };
                std::cout << "Socket failed to send" << std::endl;
            } else{
                std::cout<<"Socket failed to connect" <<std::endl;
            }
            socket.Close();
        } else{
            std::cout<<"Socket failed to  create" <<std::endl;
        }
    }
    Network::Shutdown();
    system("pause");


    return 0;
}
