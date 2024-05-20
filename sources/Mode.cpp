#include "../includes/Irc.hpp"

void Channel::mode(const std::vector<std::string> &message, Client &client) {
    std::string modes = message[0];
    if(modes.empty()){
        printMode(client, false);
        return;
    }
    bool addMode = true;
    size_t argIndex = 1;

    for (size_t i = 0; i < modes.size(); ++i) {
        char mode = modes[i];

        if (mode == '+') {
            addMode = true;
            continue;
        } else if (mode == '-') {
            addMode = false;
            continue;
        }

        switch (mode) {
            case 'i':
                _isinviteOnly = addMode;
                break;
            case 't':
                _istopicRestricted = addMode;
                break;
            case 'k':
                if(addMode){
                    if (argIndex < message.size()) {
                        _ChannelKey = message[argIndex++];
                    } else {
                        std::string errorMessage = ":" + SERVER_IP + " " + std::to_string(461) + " " + client.getNickname() + " " + "MODE +l/r/n";
                        if (send(client.getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
                            std::cerr << "Error, could not send mode message" << std::endl;
                            return;
                        }
                        return;
                    }      
                } else
                    _ChannelKey.clear(); 
                break;
            case 'o':
                break;
            case 'l':
                if (addMode) {
                    if (argIndex < message.size()) {
                        _islimit = true;
                        _limit = std::stoi(message[argIndex++]);
                    } else {
                        std::string errorMessage = ":" + SERVER_IP + " " + std::to_string(461) + " " + client.getNickname() + " " + "MODE +k/r/n";
                        if (send(client.getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
                            std::cerr << "Error, could not send mode message" << std::endl;
                            return;
                        } 
                        return;
                    }
                } else {
                    _islimit = false;
                }
                break;
            default:
                 std::string errorMessage = ":" + SERVER_IP + " " + std::to_string(472) + " " + client.getNickname() + " " + "is unkown mode char to me/r/n";
                        if (send(client.getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
                            std::cerr << "Error, could not send mode message" << std::endl;
                            return;
                        }
                break;
        }

        if (addMode && (mode == 'k' || mode == 'l')) {
            if (argIndex < args.size()) {
                ++argIndex;
            }
        }
    }
    for(_members)
    printMode(, true);
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
    if(modes) modes = "+" + modes;
    std::string modeMessage = ":" + SERVER_IP + " 324 " + client.getNickname() + " " + _name + " " + modes + args;
    if(!broadcast)
        if (send(client.getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
            std::cerr << "Error, could not send mode message" << std::endl;
            return;
        }
    else{
        for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
              if (send(it.getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
            std::cerr << "Error, could not send mode message" << std::endl;
            return;
            }
        }

    }
}