/*
 * FILE: socket.h
 * create a socket to connect
 */

#include "socket.h"

int create_socket(const char* host, int client_port) {
    int sock;
    struct sockaddr_in target_addr;
    
    bzero(target_addr, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(client_port);
    target_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //inet_pton transform decimal IP address into binary address
    inet_pton(AF_INET, host, &target_addr.sin_addr);
    
    //create socket fd
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        LOG_ERR("socket create failed");
        return -1;
    }

    int flag, res;
    flag = fcntl(sock, F_GETFL, 0); //F_GETFL = get fd's state
    CHECK(flag == -1, "fcntl error");
    flag |= O_NONBLOCK;
    res = fcntl(sock, F_SETFL, flag);
    CHECK(res == -1, "set fd error");

    connect(sock, (struct sockaddr*)&target_addr, sizeof(struct sockaddr));
    return sock;
}

