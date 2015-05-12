#ifndef SIMPLE_CHAT_H
#define SIMPLE_CHAT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>

#define SC_MAX_LISTEN_BACKLOG 10
#define SC_BUFFER_SIZE 512

enum
{
    SC_INVALID_PORT = INT_MIN,
    SC_RANGE_ERROR,
    SC_SET_SOCKET_OPTION_ERROR,
    SC_SOCKET_OPEN_ERROR,
    SC_HOST_NOT_FOUND,
    SC_CONNECT_ERROR,
    SC_LISTEN_ERROR,
    SC_ACCEPT_ERROR
};

typedef enum
{
    SC_OK = 0
} SC_status_t;

typedef struct
{
    int socket;
    int peer_socket;
    char buffer[SC_BUFFER_SIZE];
}SC_Connection;

//  FOR REFERENCE:
//  struct sockaddr_in {
//      short            sin_family;   // AF_INET
//      unsigned short   sin_port;     // e.g. htons(3490)
//      struct in_addr   sin_addr;     // see struct in_addr, below
//      char             sin_zero[8];  // zero this if you want to
//  };
//  struct in_addr {
//      unsigned long s_addr;  // load with inet_aton()
//  };
//  int inet_aton(const char *cp, struct in_addr *inp);
//      converts the Internet host address cp from the IPv4 numbers-and-dots notation  into  binary
//      form  (in  network  byte order) and stores it in the structure that inp points to.  inet_aton() returns
//      nonzero if the address is valid, zero if not.
typedef struct __sc_client
{
    int socket;         /* Client's socket file descriptor */
    struct hostent *server;
    struct sockaddr_in
        address;
    char buffer[SC_BUFFER_SIZE];
}SC_Client;

void error(char *msg);
void print_error(char *msg);

char* read_from_client(SC_Client *client);
int write_to_client(SC_Client *client,char *message);

char* read_from_server(SC_Client *client);
int write_to_server(SC_Client *client,char *message);

SC_Client* create_sc_client(void);
void destroy_sc_client(SC_Client *client);
int connect_to_host(SC_Client *client,char *host,char *port);


// ============= SERVER =============
int init_sc_server(SC_Connection *server,char *port);
int sc_listen(SC_Connection *server);
int sc_accept(SC_Connection *server);

#endif
