/*
 * FILE: socket.h
 * create a socket to connect
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int create_socket(const char* host, int client_port);

#endif
