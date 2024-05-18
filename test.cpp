std::vector<Channel>::iterator it;

for (it = _channList.begin(); it != _channList.end(); it++)
{
	if(it->getName() == channel);
	{
		if (it->isMember(clients[fd].getNickname()))
		{
			it->removeMember(clients[fd].getNickname());
			if (it.getMembers().size() == 0)
			{
				std::cout << it->getName() << " is empty now. Deleting..." << std::endl;
				_channList.erase(it);
			}
		}
	}
}

std::vector<Channel>::iterator it = _channList.begin();
std::vector<Client>::iterator clientIt = it->getOperators().begin();

for (it; it != _channList.end(); it++)
{
	if (channel == it->getName())
	{
		for (clientIt; clientIt != it->getOperators().end(); clientIt++)
		{
			if (it->clientIt->getNickname() == clients[fd].getNickname())
			{
				if (it->isMember(target) && !it->hasOperatorPrivileges(target))
				{
					it->removeMember(target);
					std::cout << target << " was kicked out of " << it->getName() << std::endl;
					printInClient("You kicked " + target + " out of " + it->getName(), fd);
				}
			}
		}
	}
}

std::vector<Channel>::iterator	it = _channList.begin();
std::map<int, Client>::iterator	clientIt = clients.begin();

for (it, it != _channlist.end(); it++)
	if (it->getName() == channel)
		break;
if (it == _channlist.end())
{
	printInClient("Couldn't invite client, check your channel's name!", fd);
	return ;
}
if (it->isMember(clients[fd].getNickname() && it->hasOperatorPrivileges(clients[fd].getNickname())))
{
	for (clientIt; clientIt != clients.end(); clientIt++)
		if (clientIt->secong.getNickname() == target)
			break
	if (clientIt == clients.end())
	{
		printInClient("Couldn't invite client, check your client's name!", fd);
		return ;
	}
	it->addMember(clientIt->second);
	printInClient(clients[fd].getNickname() + " invited " + target + " to " + channel, fd);
	printInClient(clients[fd].getNickname() + " invited you " + " to " + channel, clientIt->first);
}


std::vector<Channel>::iterator it = _channList.begin();
std::vector<Client>::iterator	clientIt = it->getMembers().begin();

for (it; it != _channList.end(); it++)
{
	if (it->getName() == target && it->isMember(clients[fd].getNickname()))
	{
		for (clientIt; clientIt != it->getMembers().end(); clientIt++)
		{
			if (fd != clientIt->getFd())
				clients[fd].sendMessage(text, clientIt->getFd())
		}
	}
}
