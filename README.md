### Simple chat program in C

#### License
- MIT License
   - **See LICENSE file**

This was started just for playing around with sockets in linux

Program uses TCP

Up until now, it just connects, sends the message and closes

#### Running server:
./sc_server < port >

#### Running client:
./sc_server < host > < port >

### TODOs
- [ ] Remove gethostbyname() and use getaddrinfo() or getnameinfo()
- [ ] SC_Client and SC_Server are almost identical, maybe use a single struct...
- [ ] Remove some duplication
- [ ] Keep communication loop
- [ ] Improve error handling
- [ ] Thread it up!