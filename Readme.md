# ft_irc

Welcome to `ft_irc`, our implementation of an IRC server. This project provides basic IRC functionalities, allowing users to connect, join channels, and interact with each other using standard IRC commands.

## Getting Started

### Running the Program

1. **Compile the Program:**
   - ```make```

2. **Run the IRC Server:**
    - ```./ircserv <port> <password>```
    - `./ircserv`: Program name.
    - `<port>`: Specify the port number for the server.
    - `<password>`: Specify the password for the server.

3. **Connect to the Server:**
    - Use `nc` (netcat) or an IRC client like HexChat to connect to the server.

4. **Enjoy our ft_irc!**

## IRC Commands

### PASS
Authenticates the client. This must be the first command sent by the client.
- ```PASS <password>```

### NICK
Sets or changes the client's nickname.
- ```NICK <nickname>```

### USER
Sets or changes the client's username.
- ```USER <username> <0> <*> <realname>```

### JOIN
Joins a specified channel.
- ```JOIN #<channel>```

### PART
Leaves a specified channel.
- ```PART #<channel> <reason>```

### KICK
Kicks a user from a channel.
- ```KICK <network> #<channel> <nickname>```
- If you're using an IRC client, it would send the network name for you.

### INVITE
Invites a user to a channel.
- ```INVITE <nickname> #<channel>```

### PRIVMSG
Sends a private message to a user or a channel.

#### User Message:
- ```PRIVMSG <nickname> <message>```

#### Channel Message:
- ```PRIVMSG #<channel> <message>```

### MODE
Sets or removes a channel's mode. Available modes are:
- `i`: Invite-only channel.
- `o`: Give/take operator privileges.
- `k`: Set/remove the channel key (password).
- `l`: Set the user limit for the channel.
- `t`: Only channel operators can change the topic.
- ```MODE #<channel> +<mode>``` to set a mode.
- ```MODE #<channel> -<mode>``` to remove a mode.

### TOPIC
Sets, changes, or shows the topic of a channel.
- ```TOPIC #<channel>``` will show the topic.
- ```TOPIC #<channel> <topic>``` will change the topic.

## Enjoy using ft_irc!

If you encounter any issues or have any questions, feel free to reach out. Happy chatting!

