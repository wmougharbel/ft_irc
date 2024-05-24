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
    - User `nc` (netcat) or an IRC client like HexChat to connect to the server.

4. **Enjoy our ft_irc!**

## IRC Commands

### PASS
Authenticates the client. This must be the first command sent by the client.
- ```PASS <password>```



