/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:03 by walid             #+#    #+#             */
/*   Updated: 2024/05/18 15:27:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(const std::string& name, const Client& firstMember) : _name(name), _isinviteOnly(false), _istopicRestricted(false){
	_ispassword = false;
	_islimit = false;
	_members.push_back(firstMember);
	_operators.push_back(firstMember);
	_topic = "Default Topic";
	_ChannelKey = "";
}

Channel::~Channel() {}

void Channel::setIsLimit(bool islimit){
	this->_islimit = islimit;
}

void Channel::setlimit(const int limit){
	this->_limit = limit;
}

int Channel::getLimit() const{
	return _limit;
}

std::string	Channel::getName() const
{
	return (_name);
}

void Channel::setTopic(const std::string& topic) {
	this->_topic = topic;
}

std::string Channel::getTopic() const {
	return _topic;
}

void Channel::setTopicRestricted(bool topicRestricted) {
	this->_istopicRestricted = topicRestricted;
}

bool Channel::isTopicRestricted() const {
	return _istopicRestricted;
}



void Channel::setInviteOnly(bool inviteOnly) {
	this->_isinviteOnly = inviteOnly;
}

bool Channel::isInviteOnly() const {
	return _isinviteOnly;
}

bool	Channel::isInvited(const std::string& name)
{
	for (std::vector<std::string>::iterator it = _invited.begin(); it != _invited.end(); ++it) {
		if (name == *it)
			return (true);
	}
	return (false);
}


void Channel::setChannelKey(const std::string& key) {
	this->_ChannelKey = key;
}

bool Channel::isPassword() const {
	return _ispassword;
}


bool Channel::checkChannelKey(const std::string& key) const {
	return key == _ChannelKey;
}

void Channel::setOperatorPrivileges(Client& member) {
	_operators.push_back(member);
}

void Channel::addMember(const Client& member){
	_members.push_back(member);
}

void Channel::removeOperatorPrivileges(const std::string& nickname) {
	if(hasOperatorPrivileges(nickname) == false){
		std::cout << nickname << " is not an operator of this Channel!" << std::endl;
		return;
	}
	for (std::vector<Client>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (it->getNickname() == nickname) {
			_operators.erase(it);
			break;
		}
	}
	if(_operators.empty() && !_members.empty()){
			setOperatorPrivileges(_members[0]);
	}
}

bool Channel::hasOperatorPrivileges(const std::string& nickname) const {
	if(_operators.empty())
		return false;
	for (std::vector<Client>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (it->getNickname() == nickname)
			return true;
	}
	return false;
}

void Channel::removeMember(const std::string& nickname) {
	if(isMember(nickname) == false){
		std::cout << nickname << " is not a member of this Channel!" << std::endl;
		return;
	}
	for (std::vector<Client>::iterator it = _members.begin(); it != _members.end(); ++it) {
		if (it->getNickname() == nickname) {
			_members.erase(it);
			break;
		}
	}
    // std::cout << "HERE" << std::endl;
	if(hasOperatorPrivileges(nickname))
		removeOperatorPrivileges(nickname);
}

bool Channel::isMember (const std::string& nickname) const {
	if(_members.empty())
		return false;
	for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		if (it->getNickname() == nickname) {
			return true;
		}
	}
	return false;
}


std::vector<Client> Channel::getMembers() const
{
	return (_members);
}

std::vector<Client> Channel::getOperators() const
{
	return (_operators);
}

Client* Channel::findClient(const std::string& nickname){
	for (size_t i = 0; i < _members.size(); i++)
	{
		if (nickname == _members[i].getNickname())
			return (&_members[i]);
	}
	return (NULL);
}