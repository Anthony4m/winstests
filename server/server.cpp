#include <iostream>
#include "../Tnet/Tnet.h"
using namespace Tnet;
int main() {
//    setsockopt()
    if(Network::Initialize()) {
        std::cout << "Initialized" << std::endl;
    }
    Network::Shutdown();


    return 0;
}
