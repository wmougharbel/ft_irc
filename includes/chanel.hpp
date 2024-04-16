#ifndef CHANEL_HPP
#define CHANEL_HPP

#include <string>
#include <vector>
#include <iostream>

class Chanel {
private:
	std::string name; 
	std::vector<std::string> members; 
	std::string topic; 
	bool isinviteOnly; 
	bool istopicRestricted;
	bool ispassword;
	bool islimit;
	int limit;
	std::string ChanelKey; 
	std::vector <std::string> operators;

public:
	Chanel(const std::string& name,const std::string& firstMember);
	~Chanel(); 


	void setTopic(const std::string& topic);
	std::string getTopic() const;
	void setTopicRestricted(bool topicRestricted);
	bool isTopicRestricted() const;

	void setInviteOnly(bool inviteOnly);
	bool isInviteOnly() const;

	void setChanelKey(const std::string& key);
	bool checkChanelKey(const std::string& key) const;

	void setOperatorPrivileges(const std::string &member);
	void removeOperatorPrivileges(const std::string &member);
	bool hasOperatorPrivileges(const std::string &member) const;

	void removeMember(const std::string& nickname);
	bool isMember(const std::string& nickname) const;

};

#endif 