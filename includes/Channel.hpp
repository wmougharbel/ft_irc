/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amurawsk <amurawsk@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:44:35 by walid             #+#    #+#             */
/*   Updated: 2024/04/28 15:00:12 by amurawsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANEL_HPP
#define CHANEL_HPP

#include <string>
#include <vector>
#include <iostream>
#include "../includes/Client.hpp"

class Channel {
private:
	std::string _name;

	std::vector<Client *>	_members;
	std::vector<Client *>	_operators;

	std::string	_topic;
	std::string	_ChannelKey; 
	std::string	_password;

	bool		_isinviteOnly;
	bool		_islimit;
	bool		_istopicRestricted;
	bool		_ispassword;
	int			_limit;

public:
	Channel(const std::string& name,const Client& firstMember);
	~Channel(); 

	void setIsLimit(bool islimit);
	void setlimit(const int limit);
	int  getLimit();
	

	void setTopic(const std::string& topic);
	std::string getTopic() const;
	void setTopicRestricted(bool topicRestricted);
	bool isTopicRestricted() const;

	void setInviteOnly(bool inviteOnly);
	bool isInviteOnly() const;

	void setChannelKey(const std::string& key);
	bool IsPassword() const;
	bool checkChannelKey(const std::string& key) const;

	void setOperatorPrivileges(const Client &member);
	void removeOperatorPrivileges(const std::string &member);
	bool hasOperatorPrivileges(const std::string &member) const;

	void addMember(const Client &member);
	void removeMember(const std::string& nickname);
	bool isMember(const std::string& nickname) const;
};

#endif
