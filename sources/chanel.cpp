#include "Chanel.hpp"

Chanel::Chanel(const std::string& name, const std::string& firstMember) : name(name), isinviteOnly(false), istopicRestricted(false), ispassword(false), limit(0) {
	members.push_back(firstMember);
	operators.push_back(firstMember);
}

Chanel::~Chanel() {}

void Chanel::setlimit(const int limit){
	this->limit == limit;
}


void Chanel::setTopic(const std::string& topic) {
	this->topic = topic;
}

std::string Chanel::getTopic() const {
	return topic;
}

void Chanel::setTopicRestricted(bool topicRestricted) {
	this->istopicRestricted = topicRestricted;
}

bool Chanel::isTopicRestricted() const {
	return istopicRestricted;
}



void Chanel::setInviteOnly(bool inviteOnly) {
	this->isinviteOnly = inviteOnly;
}

bool Chanel::isInviteOnly() const {
	return isinviteOnly;
}



void Chanel::setChanelKey(const std::string& key) {
	this->ChanelKey = key;
}

bool Chanel::checkChanelKey(const std::string& key) const {
	return key == ChanelKey;
}

void Chanel::setOperatorPrivileges(const std::string& member) {
	operators.push_back(member);
}

void Chanel::removeOperatorPrivileges(const std::string& member) {
	if(hasOperatorPrivileges(member) == false){
		std::cout << member << " is not an operator of this chanel!" << std::endl;
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

bool Chanel::hasOperatorPrivileges(const std::string& member) const {
	if(operators.empty())
		return false;
	for (std::vector<std::string>::const_iterator it = operators.begin(); it != operators.end(); ++it) {
		if (*it == member)
			return true;
	}
	return false;
}

void Chanel::removeMember(const std::string& nickname) {
	if(isMember(nickname) == false){
		std::cout << nickname << " is not a member of this chanel!" << std::endl;
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

bool Chanel::isMember(const std::string& nickname) const {
	if(members.empty())
		return false;
	for (std::vector<std::string>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (*it == nickname) {
			return true;
		}
	}
	return false;
}