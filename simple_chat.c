#include "simple_chat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

// 1-65535  all ports
// 1-1023   reserved (root)
int validade_and_convert_port(char *port)
{
    int port_number = 0;

    if(strlen(port) > 5)
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

    if(port_number > 65535)
    {
        return SC_RANGE_ERROR;
    }

    return port_number;
}
