#include "simple_chat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void print_error(char *msg)
{
    perror(msg);
}

void init_client(SC_Client *client)
{
    if(client == NULL)
    {
        return;
    }

    memset(client,0,sizeof(SC_Client));
    client->address.sin_family = AF_INET;   /* IPV4 */

    client->socket = -1;
}

// 1-65535  all ports
// 1-1023   reserved (root)
int validade_and_convert_port(char *port)
{
    int port_number = 0;

    if((port == NULL) || strlen(port) > 5)
    {
        return SC_INVALID_PORT;
    }

    while(*port)
    {
        port_number *= 10;

        if(!isdigit(*port))
        {
            return SC_INVALID_PORT;
        }

        port_number += (*port) - '0';
        ++port;
    }

    if((port_number > 65535) || (port_number == 0))
    {
        return SC_RANGE_ERROR;
    }

    return port_number;
}

int sc_write_to_socket(int socket,char *message)
{
    int bytes_written;

    bytes_written = write(socket,message,strlen(message));

    return bytes_written;
}

int sc_read_from_socket(int socket,char *buffer,int buffer_size)
{
    int bytes_read;

    memset(buffer,'\0',buffer_size);
    bytes_read = read(socket,buffer,255);

    return bytes_read;
}

char* read_from_client(SC_Client *client)
{
    int bytes_read;

    bytes_read = sc_read_from_socket(client->socket,client->buffer,SC_BUFFER_SIZE);
    if (bytes_read < 0)
    {
        print_error("ERROR reading from socket");
        return NULL;
    }

    return client->buffer;
}

int write_to_client(SC_Client *client,char *message)
{
    return sc_write_to_socket(client->socket,message);
}

char* read_from_server(SC_Client *client)
{
    int bytes_read;

    bytes_read = sc_read_from_socket(client->socket,client->buffer,SC_BUFFER_SIZE);
    if (bytes_read < 0)
    {
        print_error("ERROR reading from socket");
        return NULL;
    }

    return client->buffer;
}

int write_to_server(SC_Client *client,char *message)
{
    return sc_write_to_socket(client->socket,message);
}

SC_Client* create_sc_client(void)
{
    SC_Client *new_client;

    new_client = (SC_Client *) malloc(sizeof(SC_Client));

    if(new_client != NULL)
    {
        init_client(new_client);
    }

    return new_client;
}

void destroy_sc_client(SC_Client *client)
{
    if(client == NULL)
    {
        return;
    }

    if(client->socket > 0)
    {
        close(client->socket);
    }
    free(client);
}

int connect_to_host(SC_Client *client,char *host,char *port)
{
    struct hostent *server;
    int port_number = validade_and_convert_port(port);

    if(port_number < 0)
    {
        return port_number;
    }

    client->socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client->socket < 0)
    {
        print_error("ERROR opening socket");
        return SC_SOCKET_OPEN_ERROR;
    }

    /*
        The gethostbyname*(), gethostbyaddr*(), herror(), and hstrerror() functions are obsolete.  Applications
        should use getaddrinfo(3), getnameinfo(3), and gai_strerror(3) instead.
    */
    server = gethostbyname(host);
    if(server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        return SC_HOST_NOT_FOUND;
    }

    memcpy( (char *)&client->address.sin_addr.s_addr,
            (char *)server->h_addr,
            server->h_length);

    client->address.sin_port = htons(port_number);
    if(connect(client->socket,(struct sockaddr *)&client->address,sizeof(struct sockaddr_in)) < 0)
    {
        return SC_CONNECT_ERROR;
    }

    return SC_OK;
}


//  ============== SERVER ==============

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sc_set_server_hints(struct addrinfo *hints)
{
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;        // Allow IPV4 and IPV6
    hints->ai_socktype = SOCK_STREAM;    // TCP

    // If the AI_PASSIVE flag is specified in hints.ai_flags,
    // and node is NULL,
    // then the returned socket addresses will be suitable for
    // bind(2)ing a socket that will accept(2) connections.
    hints->ai_flags = AI_PASSIVE;
}

int init_sc_server(SC_Connection *server,char *port)
{
    struct addrinfo hints;
    struct addrinfo *result_list, *node;
    int result;
    int yes = 1;

    sc_set_server_hints(&hints);

    result = getaddrinfo(NULL, port, &hints, &result_list);

    if(result != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        return SC_HOST_NOT_FOUND;
    }

    // loop through all the results trying to bind
    for(node = result_list; node != NULL; node = node->ai_next)
    {
        if((server->socket = socket(node->ai_family,
                                    node->ai_socktype,
                                    node->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        /* int setsockopt(  int sockfd,
                            int level,
                            int optname,
                            const void *optval,
                            socklen_t optlen);
            Sets REUSEADDR on the socket (yes == 1)
        */
        if(setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");

            // Free linked list
            freeaddrinfo(result_list);
            return SC_SET_SOCKET_OPTION_ERROR;
        }

        if(bind(server->socket, node->ai_addr, node->ai_addrlen) == -1)
        {
            close(server->socket);
            perror("server: bind");
            continue;
        }

        break;
    }

    if(result_list != NULL)
    {
        // Free linked list
        freeaddrinfo(result_list);
    }

    if(node == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    return SC_OK;
}

int sc_listen(SC_Connection *server)
{
    if(listen(server->socket, SC_MAX_LISTEN_BACKLOG) == -1)
    {
        perror("server(listen)");
        return SC_LISTEN_ERROR;
    }

    return SC_OK;
}

void get_address_str(struct sockaddr_storage *address,char *address_str,size_t str_size)
{
    /* convert IPv4 and IPv6 addresses from binary to text form */
    inet_ntop(  address->ss_family,
                get_in_addr((struct sockaddr *)address),
                address_str,
                str_size);
}

int sc_accept(SC_Connection *server)
{
    struct sockaddr_storage peer_address;
    socklen_t sin_size = sizeof(struct sockaddr_storage);
    char peer_address_str[INET6_ADDRSTRLEN];

    server->peer_socket = accept(server->socket, (struct sockaddr *)&peer_address, &sin_size);

    if(server->peer_socket == -1)
    {
        perror("accept");
        return SC_ACCEPT_ERROR;
    }

    get_address_str(&peer_address,peer_address_str,sizeof(peer_address_str));
    printf("server: got connection from %s\n", peer_address_str);

    return SC_OK;
}