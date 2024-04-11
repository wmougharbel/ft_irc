#include "../includes/irc.hpp"

int main(int argc, char **argv)
{
    try
    {
        if (argc != 3)
            throw(std::runtime_error("Incorrect number of arguments!\nUsage: ./ircserv <port> <password>\ne.g Port = 6667\te.g Password = P@$$w0rd"));
        validateInput(argv[1], argv[2]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}