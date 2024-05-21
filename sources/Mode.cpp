#include "../includes/Irc.hpp"

class Channel;
class Client;
void Channel::mode(const std::vector<std::string> &message, Client &client) {
    std::string modes = message[2];
    if(modes.empty())
    {
        printMode(client, false);
        return;
    }
    if (!hasOperatorPrivileges(client.getNickname())) 
    {
        std::string errorMessage = std::string(":") + SERVER_IP + " 482 " + client.getNickname() + " #" + _name + " :You're not channel operator\r\n";
        if (send(client.getFd(), errorMessage.c_str(), errorMessage.length(), 0) < 0) 
        {
            std::cerr << "Error, could not send error message" << std::endl;
        }
        return;
    }
    bool addMode = true;
    size_t argIndex = 3;
    for (size_t i = 0; i < modes.size(); ++i) 
    {
        char mode = modes[i];

        if (mode == '+') {
            addMode = true;
            continue;
        } else if (mode == '-') {
            addMode = false;
            continue;
        }

        switch (mode) 
        {
            case 'i':
                _isinviteOnly = addMode;
                break;
            case 't':
                _istopicRestricted = addMode;
                break;
            case 'k':
                if(addMode)
                {
                    if (argIndex < message.size()) 
                    {
                        _ChannelKey = message[argIndex++];
                    } else continue;     
                } 
                else
                    _ChannelKey.clear(); 
                break;
            case 'o':
                if(argIndex < message.size())
                {
                    if(addMode)
                    {
                        if(!hasOperatorPrivileges(message[argIndex]))
                        {
                            if(!isMember(message[argIndex]))
                            {
                                std::string errorMessage = std::string(":") + SERVER_IP + " " + "441" + " " + client.getNickname() + " " +_name + " " + message[argIndex++] + ":User is not on that channel\r\n";
                                if (send(client.getFd(), errorMessage.c_str(), errorMessage.length(), 0) < 0) 
                                {
                                    std::cerr << "Error, could not send mode message" << std::endl;
                                    return;
                                }
                            }
                            setOperatorPrivileges(*findClient(message[argIndex++]));
                        }
                    }
                    else
                    {
                        if(hasOperatorPrivileges(message[argIndex]))
                            removeOperatorPrivileges(message[argIndex]);
                    }
                } else continue;
                break;
            case 'l':
                if (addMode) 
                {
                    if (argIndex < message.size())
                    {
                        _islimit = true;
                        _limit = std::atoi(message[argIndex++].c_str());
                    } else continue;
                }
                else 
                    _islimit = false;
                break;
            default:
                 std::string errorMessage = std::string(":") + SERVER_IP + " " + "472" + " " + client.getNickname() + " " + "is unkown mode char to me\r\n";
                        if (send(client.getFd(), errorMessage.c_str(), errorMessage.length(), 0) < 0) 
                        {
                            std::cerr << "Error, could not send mode message" << std::endl;
                            return;
                        }
                break;
        }
    }
    printMode(client, true);
}

void Channel::printMode(Client &client, bool broadcast)
{
    std::string modes;
    std::string args;
    std::stringstream ss;
    if(_isinviteOnly) modes += "i";
    if(_istopicRestricted) modes += "t";
    if(_islimit){
        modes += "l";
        ss << _limit;
        args += " " + ss.str();
    }
    if(!_ChannelKey.empty()) modes+= "k";
    if(!modes.empty())
        modes = "+" + modes;
        
    std::string modeMessage = std::string(":") + SERVER_IP + " 324 " + client.getNickname() + " " + _name + " " + modes + args;
    if(!broadcast)
    {
        if (send(client.getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
            std::cerr << "Error, could not send mode message" << std::endl;
            return;
        }
    } else
    {
        for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
            modeMessage = std::string(":") + SERVER_IP + " 324 " + it->getNickname() + " " + _name + " " + modes + args;
            if (send(it->getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
                std::cerr << "Error, could not send mode message" << std::endl;
                return;
            }
        }

    }
}