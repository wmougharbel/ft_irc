/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loandrad <loandrad@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:44:35 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 19:50:26 by loandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANEL_HPP
#define CHANEL_HPP

#include <string>
#include <vector>
#include <iostream>

class Channel {
private:
	std::string name; 
	std::vector<std::string> members;
	std::string topic; 
	bool isinviteOnly; 
	bool istopicRestricted;
	bool ispassword;
	int limit;
	std::string ChannelKey; 
	std::vector <std::string> operators;

public:
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

	void removeMember(const std::string& nickname);
	bool isMember(const std::string& nickname) const;

};

#endif
