#include "../includes/irc.hpp"

bool    isArgumentNumeric(std::string argument)
{
    for (size_t i = 0; i < argument.length(); i++)
        if (!isdigit(argument[i]))
            return (false);
    return (true);
}

bool    getArgumentLength(std::string argument)
{
    if (argument.length() != 4)
        return (false);
    return (true);
}

void    validateInput(std::string argument)
{
    if (!isArgumentNumeric(argument))
        throw (std::runtime_error("Argument should only contain numbers!"));
    if (!getArgumentLength(argument))
        throw (std::runtime_error("Argument should contain 4 numbers!"));
}