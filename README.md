
# TCP-chat

This is a small command line chat program based on TCP.

## Instructions

- To compile, type `make`.
- To run the server-side, type `./program.o PORT` where `PORT` is the TCP port the server listens on.
- To run the client-side, type `./program.o IP PORT NAME` where `IP` is the ip-address of the computer running the server, `PORT` is the TCP port the server is configured to listen to, and `NAME` the name you want this client to have in the chat. *The server must be started before the clients.*
- Exit a server or a client with ctrl-c.

## Example run

The commands `./program.o 1234`, `./program.o 127.0.0.1 1234 Alice`, `./program.o 127.0.0.1 1234 Bob`, and `./program.o 127.0.0.1 1234 Charlie` are typed on the local machine. Then the users start chatting.

Alice's terminal:

```
Is it time for tea?
Bob: I don't think so, but let's hear what Charlie has to say.
Charlie: It's always time for tea.
```

Bob's terminal:

```
Alice: Is it time for tea?
I don't think so, but let's hear what Charlie has to say.
Charlie: It's always time for tea.
```

Charlie's terminal:

```
Alice: Is it time for tea?
Bob: I don't think so, but let's hear what Charlie has to say.
It's always time for tea.
```