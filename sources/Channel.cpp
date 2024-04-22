/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amurawsk <amurawsk@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:03 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 21:38:19 by amurawsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(const std::string& name, const Client& firstMember) : name(name), _isinviteOnly(false), _istopicRestricted(false), _ispassword(false), _limit(-1){
	_members.push_back(firstMember);
	_operators.push_back(firstMember);
}

Channel::~Channel() {}

void Channel::setlimit(const int limit){
	this->_limit = limit;
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



void Channel::setChannelKey(const std::string& key) {
	this->_ChannelKey = key;
}

bool Channel::checkChannelKey(const std::string& key) const {
	return key == _ChannelKey;
}

void Channel::setOperatorPrivileges(const Client& member) {
	_operators.push_back(member);
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
		for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end();++it) {
			if (it->getNickname() == nickname) 
				continue;
			setOperatorPrivileges(*it);
		}
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
	if(hasOperatorPrivileges(nickname))
		removeOperatorPrivileges(nickname);
	if(_members.empty())
		delete this;
}

bool Channel::isMember(const std::string& nickname) const {
	if(_members.empty())
		return false;
	for (std::vector<Client>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		if (it->getNickname() == nickname) {
			return true;
		}
	}
	return false;
}
