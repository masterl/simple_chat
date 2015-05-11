// The MIT License (MIT)

// Copyright (c) 2015 Leonardo de Oliveira Lourenço

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>

#include "simple_chat.h"

int main(int argc, char *argv[])
{
    char *msg;
    int bytes_written;

    SC_Server *server;
    SC_Client *client;

    if (argc < 2)
    {
        fprintf(stderr,"Usage:\n\t%s <port>\n",argv[0]);
        exit(1);
    }

    server = create_sc_server();

    if(init_sc_server(server,argv[1]) != SC_OK)
    {
        printf("Couldn't init simple chat server!\n");
        exit(EXIT_FAILURE);
    }

    if(start_sc_server(server) != SC_OK)
    {
        print_error("Couldn't start server");
    }
    else
    {
        client = sc_accept_connection(server);

        if(client != NULL)
        {
            msg = read_from_client(client);
            printf("Here is the message: %s\n",msg);
            bytes_written = write_to_client(client,"I got your message");
            if(bytes_written < 0)
            {
                print_error("ERROR writing to socket");
            }
        }
    }

    destroy_sc_server(server);

    return EXIT_SUCCESS;

    return 0; 
}