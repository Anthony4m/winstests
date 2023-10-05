#include <iostream>
#include "../Tnet/Tnet.h"
using namespace Tnet;
int main() {
    if(Network::Initialize()) {
        std::cout << "Initialized" << std::endl;
        //server to bind port 1996

        Socket socket;
        if (socket.Create() == TResults::T_Success){
            std::cout<<"Socket Successfully created" <<std::endl;
            // server to listen for connections on 1995
            if (socket.Listen(IPEndpoint("0.0.0.0",1996)) == TResults::T_Success){
                std::cout<<"Socket Successfully Listening to 1996" <<std::endl;
                Socket clientConnection;
                if (socket.Accept(clientConnection) == TResults::T_Success){
                    std::cout<<"Socket Successfully Accepted" <<std::endl;
                    char buffer[1024];
                    int bytesReceived = 0;
                    auto result = TResults::T_Success;
                    while (result == TResults::T_Success){
                        result = clientConnection.RecvAll(buffer,sizeof(buffer));
                        std::cout<<"Receiving Data.....: "<<buffer<<std::endl;
                        if (result != TResults::T_Success){
                            std::cout<<"Socket failed to receive" <<std::endl;
                            break;
                            std::cout<<buffer<<std::endl;
                        }
                    }
                }
            } else{
                std::cout<<"Socket failed to listen" <<std::endl;
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
