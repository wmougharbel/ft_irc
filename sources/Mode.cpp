#include "../includes/Irc.hpp"
void Server::mode(std::vector<std::string> &message){
    
}
void Channel:mode(std::vector<std::string> &message, Client &client ){
    
}

void Channel::printMode(Client &client)
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
    if (send(client.getFd(), modeMessage.c_str(), modeMessage.length(), 0) < 0) {
        std::cerr << "Error, could not send mode message" << std::endl;
        return;
    }
}