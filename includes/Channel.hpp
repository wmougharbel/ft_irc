/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:44:35 by walid             #+#    #+#             */
/*   Updated: 2024/04/28 16:10:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANEL_HPP
#define CHANEL_HPP

#include <string>
#include <vector>
#include <iostream>

// class Client;

class Channel {
private:
	std::string name; 
	std::string topic; 

	bool isinviteOnly; 
	bool istopicRestricted;
	bool ispassword;
	int limit;
	std::string ChannelKey; 
	std::vector <std::string> operators;

public:
	std::vector<std::string> members;
	Channel(const std::string& name,const std::string& firstMember);
	~Channel(); 

	void setlimit(const int limit);

	void setTopic(const std::string& topic);
	std::string getTopic() const;
	void setTopicRestricted(bool topicRestricted);
	bool isTopicRestricted() const;

	void setInviteOnly(bool inviteOnly);
	bool isInviteOnly() const;

	void setChannelKey(const std::string& key);
	bool checkChannelKey(const std::string& key) const;

	void setOperatorPrivileges(const std::string &member);
	void removeOperatorPrivileges(const std::string &member);
	bool hasOperatorPrivileges(const std::string &member) const;

	void addMember(const std::string &nickname);
	void removeMember(const std::string& nickname);
	bool isMember(const std::string& nickname) const;

};

#endif
