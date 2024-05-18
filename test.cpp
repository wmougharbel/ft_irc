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