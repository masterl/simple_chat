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
#include <string.h>

#include "simple_chat.h"

int main(int argc, char *argv[])
{
    int n;
    char buffer[256];
    char *message;

    SC_Client *client;

    if(argc < 3)
    {
        fprintf(stderr,"Usage:\n\t%s <hostname> <port>\n", argv[0]);
        exit(0);
    }

    client = create_sc_client();

    if(connect_to_host(client,argv[1],argv[2]) != SC_OK)
    {
        print_error("ERROR connecting");
    }
    else
    {
        printf("Please enter the message: ");
        memset(buffer,'\0',256);
        fgets(buffer,255,stdin);
        n = write_to_server(client,buffer);
        if (n < 0) 
        {
            error("ERROR writing to socket");
        }

        message = read_from_server(client);
        if (message == NULL)
        {
            error("ERROR reading from socket");
        }
        printf("%s\n",message);
    }

    destroy_sc_client(client);

    return EXIT_SUCCESS;
}