#include <iostream>
#include "../Tnet/Tnet.h"
using namespace Tnet;
int main() {
    if(Network::Initialize()) {
        std::cout << "Initialized" << std::endl;
        IPEndpoint test("www.google.com", 8080);
        if (test.GetIPversion() == IPVersion::IPv4){
            std::cout<<"Hostname "<< test.GetHostname() <<std::endl;
            std::cout<<"IP Address "<< test.GetIPAddress() <<std::endl;
            std::cout<<"Port "<< test.GetPort() <<std::endl;
            std::cout<<"IPv4 " <<std::endl;
            for(auto & digit : test.GetIPBytes()){
                std::cout<<(int)digit <<std::endl;
            }
        } else{
            std::cout<<"This is not an IPv4" <<std::endl;
        }
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
