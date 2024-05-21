/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:44:35 by walid             #+#    #+#             */
/*   Updated: 2024/05/13 10:48:41 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANEL_HPP
#define CHANEL_HPP

#include <string>
#include <vector>
#include <iostream>
#include "../includes/Client.hpp"

class Client;

class Channel {
private:
	std::string _name;

	
	std::vector<Client >	_members;
	std::vector<Client >	_operators;
	std::vector<std::string >	_invited;

	std::string	_topic;
	std::string	_ChannelKey; 

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
	int  getLimit() const;
	

	void setTopic(const std::string& topic);
	std::string getTopic() const;
	void setTopicRestricted(bool topicRestricted);
	bool isTopicRestricted() const;

	void setInviteOnly(bool inviteOnly);
	bool isInviteOnly() const;
	bool isInvited(const std::string& name);

	void setChannelKey(const std::string& key);
	bool isPassword() const;
	bool checkChannelKey(const std::string& key) const;

    void setOperatorPrivileges(const Client& member);
    void removeOperatorPrivileges(const std::string &member);
	bool hasOperatorPrivileges(const std::string &member) const;

	void addMember(const Client &member);
	void removeMember(const std::string& nickname);
	bool isMember(const std::string& nickname) const;

	std::vector<Client> getMembers() const;
	std::vector<Client> getOperators() const;

	std::string	getName() const;
	void printMode(Client &client, bool broadcast);
	void mode(const std::vector<std::string> &message, Client &client);
	Client findClient(const std::string& string nickname) const;


};

#endif
