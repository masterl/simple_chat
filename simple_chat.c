#include "simple_chat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>

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

void init_server(SC_Server *server)
{
    if(server == NULL)
    {
        return;
    }

    memset(server,0,sizeof(SC_Server));
    server->address.sin_family = AF_INET;           /* IPV4 */
    server->address.sin_addr.s_addr = INADDR_ANY;   /* accept any IP */

    server->socket = -1;
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

SC_Server* create_sc_server(void)
{
    SC_Server *new_server;

    new_server = (SC_Server *) malloc(sizeof(SC_Server));

    if(new_server != NULL)
    {
        init_server(new_server);
        new_server->socket = socket(AF_INET, SOCK_STREAM, 0);
    }

    return new_server;
}

void destroy_sc_server(SC_Server *server)
{
    if(server == NULL)
    {
        return;
    }

    if(server->socket > 0)
    {
        close(server->socket);
    }
    free(server);
}

int init_sc_server(SC_Server *server,char *port)
{
    int port_number = validade_and_convert_port(port);

    if(port_number < 0)
    {
        return port_number;
    }

    if(server->socket < 0)
    {
        return SC_SOCKET_OPEN_ERROR;
    }

    server->address.sin_port = htons(port_number);

    if( bind(server->socket, (struct sockaddr *) &server->address, sizeof(server->address)) < 0)
    {
        error("ERROR on binding");
    }

    return SC_OK;
}

int start_sc_server(SC_Server *server)
{
    return listen(server->socket,SC_MAX_LISTEN_BACKLOG);
}

SC_Client* sc_accept_connection(SC_Server *server)
{
    SC_Client *client;
    socklen_t client_length = sizeof(struct sockaddr_in);

    client = create_sc_client();

    if(client != NULL)
    {
        // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

        client->socket = accept(server->socket, (struct sockaddr *) &client->address, &client_length);

        if(client->socket < 0)
        {
            print_error("ERROR accepting connection");
            destroy_sc_client(client);
            client = NULL;
        }
    }

    return client;
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
