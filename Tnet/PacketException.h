//
// Created by pepos on 10/25/2023.
//

#ifndef WINSTESTS_PACKETEXCEPTION_H
#define WINSTESTS_PACKETEXCEPTION_H

#include <string>

namespace Tnet{
    class PacketException{
        public:
            PacketException(std::string exception):exception(exception){}
            const char * what() const noexcept {
                return exception.c_str();
            }
            std::string ToString() const {
                return exception;
            }
        private:
            std::string exception;
    };
}
#endif //WINSTESTS_PACKETEXCEPTION_H
