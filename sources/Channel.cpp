/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:03 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 18:48:03 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(const std::string& name, const std::string& firstMember) : name(name), isinviteOnly(false), istopicRestricted(false), ispassword(false), limit(0) {
	members.push_back(firstMember);
	operators.push_back(firstMember);
}

Channel::~Channel() {}

void Channel::setlimit(const int limit){
	this->limit = limit;
}


void Channel::setTopic(const std::string& topic) {
	this->topic = topic;
}

std::string Channel::getTopic() const {
	return topic;
}

void Channel::setTopicRestricted(bool topicRestricted) {
	this->istopicRestricted = topicRestricted;
}

bool Channel::isTopicRestricted() const {
	return istopicRestricted;
}



void Channel::setInviteOnly(bool inviteOnly) {
	this->isinviteOnly = inviteOnly;
}

bool Channel::isInviteOnly() const {
	return isinviteOnly;
}



void Channel::setChannelKey(const std::string& key) {
	this->ChannelKey = key;
}

bool Channel::checkChannelKey(const std::string& key) const {
	return key == ChannelKey;
}

void Channel::setOperatorPrivileges(const std::string& member) {
	operators.push_back(member);
}

void Channel::removeOperatorPrivileges(const std::string& member) {
	if(hasOperatorPrivileges(member) == false){
		std::cout << member << " is not an operator of this Channel!" << std::endl;
		return;
	}
	for (std::vector<std::string>::iterator it = operators.begin(); it != operators.end(); ++it) {
		if (*it == member) {
			operators.erase(it);
			break;
		}
	}
	if(operators.empty() && !members.empty()){
		for (std::vector<std::string>::const_iterator it = members.begin(); it != members.end();++it) {
			if (*it == member) 
				continue;
			setOperatorPrivileges(member);
		}
	}
}

bool Channel::hasOperatorPrivileges(const std::string& member) const {
	if(operators.empty())
		return false;
	for (std::vector<std::string>::const_iterator it = operators.begin(); it != operators.end(); ++it) {
		if (*it == member)
			return true;
	}
	return false;
}

void Channel::removeMember(const std::string& nickname) {
	if(isMember(nickname) == false){
		std::cout << nickname << " is not a member of this Channel!" << std::endl;
		return;
	}
	for (std::vector<std::string>::iterator it = members.begin(); it != members.end(); ++it) {
		if (*it == nickname) {
			members.erase(it);
			break;
		}
	}
	if(hasOperatorPrivileges(nickname))
		removeOperatorPrivileges(nickname);
	if(members.empty())
		delete this;
}

bool Channel::isMember(const std::string& nickname) const {
	if(members.empty())
		return false;
	for (std::vector<std::string>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (*it == nickname) {
			return true;
		}
	}
	return false;
}