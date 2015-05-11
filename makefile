# The MIT License (MIT)

# Copyright (c) 2015 Leonardo de Oliveira Lourenço

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#   =======================
#  ||    ROOT MAKEFILE    ||
#   =======================
#

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	Compiler:
#--------------------------------------------------------------------------
CC = gcc

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	Compilation flags
#--------------------------------------------------------------------------
COMPFLAGS = -Wall

LINKFLAGS = 

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Executables
#--------------------------------------------------------------------------
SERVER := sc_server
CLIENT := sc_client

all: $(SERVER) $(CLIENT)
	@echo -e ' \nExecutables generated!'
	@echo -e ' \t$(SERVER)\n\t$(CLIENT)'

$(SERVER): server.o simple_chat.o
	$(CC) $(COMPFLAGS) server.o simple_chat.o -o $(SERVER) $(LINKFLAGS)

$(CLIENT): client.o simple_chat.o
	$(CC) $(COMPFLAGS) client.o simple_chat.o -o $(CLIENT) $(LINKFLAGS)

server.o: server.c simple_chat.h
	$(CC) $(COMPFLAGS) -c server.c

client.o: client.c simple_chat.h
	$(CC) $(COMPFLAGS) -c client.c

simple_chat.o: simple_chat.c simple_chat.h
	$(CC) $(COMPFLAGS) -c simple_chat.c

FORCE:

#  ===================================
#  ||    VIDYA MAKE FILE >FIM<      ||
#  ===================================
