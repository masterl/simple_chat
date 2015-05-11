#ifndef SIMPLE_CHAT_H
#define SIMPLE_CHAT_H

enum
{
    SC_INVALID_PORT = -1,
    SC_RANGE_ERROR = -2
};

void error(char *msg);

int validade_and_convert_port(char *port);

#endif
