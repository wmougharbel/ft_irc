#ifndef MOCKSERVER_HPP
# define MOCKSERVER_HPP

#include "irc.hpp"

class MockServer
{
    private:   
        std::string _password;
    
    public:
        MockServer();
        ~MockServer();
};

#endif