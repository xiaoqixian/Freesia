/*
 * FILE: socket.h
 * create a socket to connect
 */

#include "socket.h"
#include "debug.h"

int create_socket(const char* host, int client_port) {
    int sock;
    struct sockaddr_in target_addr;
    
    memset(&target_addr, 0, sizeof(target_addr));
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

    /*
    int flag, res;
    flag = fcntl(sock, F_GETFL, 0); //F_GETFL = get fd's state
    CHECK(flag == -1, "fcntl error");
    flag |= O_NONBLOCK;
    res = fcntl(sock, F_SETFL, flag);
    CHECK(res == -1, "set fd error");
    */

    int res;
    res = connect(sock, (struct sockaddr*)&target_addr, sizeof(struct sockaddr));
    if (res != 0) {
        LOG_ERR("socket connect error");
        return -1;
    }
    return sock;
}

