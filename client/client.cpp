#include <iostream>
#include "../Tnet/Tnet.h"
using namespace Tnet;
int main() {
    if(Network::Initialize()) {
        std::cout << "Initialized" << std::endl;
        Socket socket;
        if (socket.Create() == TResults::T_Success){
            std::cout<<"Socket Successfully created" <<std::endl;
            socket.Close();
        } else{
            std::cout<<"Socket failed to  create" <<std::endl;
        }
    }
    Network::Shutdown();


    return 0;
}
