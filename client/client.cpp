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
                char buffer[1024];
                strcpy(buffer,"Hello World from client");
                auto result = TResults::T_Success;
                while (result == TResults::T_Success){
                    result = socket.SendAll(buffer,sizeof(buffer));
                    std::cout<<"Sending Data.....: "<<buffer<<std::endl;
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
