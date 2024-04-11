#include "../includes/irc.h"

int main(int argc, char **argv)
{
    try
    {
        if (argc != 3)
            throw(std::runtime_error("Incorrect number of arguments!"));
        (void)argv;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}