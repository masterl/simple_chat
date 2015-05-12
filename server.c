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
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>


#include "simple_chat.h"

int main(int argc, char *argv[])
{
    SC_Connection server;
    SC_Connection peer;

    if (argc < 2)
    {
        fprintf(stderr,"Usage:\n\t%s <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    if(init_sc_server(&server,argv[1]) != SC_OK)
    {
        fprintf(stderr,"server: couldn't initiate server\n");
        exit(EXIT_FAILURE);
    }

    if(sc_listen(&server) != SC_OK)
    {
        exit(EXIT_FAILURE);
    }

    printf("server: waiting for connections...\n");

    while(1)
    {
        if(sc_accept(&server,&peer) != SC_OK)
        {
            perror("accept");
            continue;
        }


        if(send(peer.socket, "Hello, world!", 13, 0) == -1)
        {
            perror("send");
        }

        close(peer.socket);
    }

    close(server.socket);

    return 0;
}