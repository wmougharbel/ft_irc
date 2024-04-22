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
        
        //getters
        std::string getPassword() const;

        //setters
        void setPassword(std::string password);
};

#endif