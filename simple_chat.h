#ifndef SIMPLE_CHAT_H
#define SIMPLE_CHAT_H

#include <sys/socket.h>
#include <netinet/in.h>

#define SC_MAX_LISTEN_BACKLOG 5
#define SC_BUFFER_SIZE 512

enum
{
    SC_INVALID_PORT = -1,
    SC_RANGE_ERROR = -2,
    SC_SOCKET_OPEN_ERROR = -3
};

typedef enum __sc_status
{
    SC_OK = 0
} SC_status_t;

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
typedef struct __sc_server
{
    int socket;         /* Server's socket file descriptor */
    struct sockaddr_in
        address;
    char buffer[SC_BUFFER_SIZE];
} SC_Server;

typedef struct __sc_client
{
    int socket;         /* Client's socket file descriptor */
    struct sockaddr_in
        address;
    char buffer[SC_BUFFER_SIZE];
}SC_Client;

void error(char *msg);
void print_error(char *msg);

char* read_from_client(SC_Client *client);
int write_to_client(SC_Client *client,char *message);

SC_Server* create_sc_server(void);
void destroy_sc_server(SC_Server *server);
int init_sc_server(SC_Server *server,char *port);
int start_sc_server(SC_Server *server);
SC_Client* sc_accept_connection(SC_Server *server);

SC_Client* create_sc_client(void);
void destroy_sc_client(SC_Client *client);

#endif
