#include "../includes/irc.hpp"

MockServer::MockServer(){}

MockServer::~MockServer(){}

std::string MockServer::getPassword() const
{
    return (_password);
}

void    MockServer::setPassword(std::string password)
{
    _password = password;
}