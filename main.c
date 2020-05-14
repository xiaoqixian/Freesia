/*
 * (C) Radim Kolar 1997-2004
 * 
 * Server concurrent test program, use threadpool
 */

#include <unistd.h>
#include <sys/rpc.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

#include "socket.h"
#include "threadpool.h"

#define PROGRAM_VERSION "1.5"
#define PROGRAM_NAME "FREESIA"
#define REQUEST_SIZE 2048
#define MAX_HOST_LEN 100
#define METHOD_GET 1
#define METHOD_POST 2
#define METHOD_HEAD 3

char request[REQUEST_SIZE];
char host[MAX_HOST_LEN];
int method;
char* hostname;
int port;
int thread_num = 10;

void usage() {
    fprintf(stderr, "options\n"
            "-a | set ip address, required"
            "-p | set connect port, required"
            "-n | set thread num, optional, default 10");
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        usage();
        return -1;
    }
    

    int opt;
    char* op_string = "a:p:n::h";
    while ((opt = getopt(argc, argv, op_string)) != -1) {
        switch (opt) {
            case 'a':
                hostname = optarg;
                break;
            case 'n':
                thread_num = atoi(optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                usage();
                break;
        }
    }
    
    //create threadpool
    threadpool_t* tp = NULL;
    tp = threadpool_init(thread_num);
    CHECK(tp == NULL, "threadpool init error");
    
    //build a request, shared by all threads
    build_request(hostname);
    
    
}

void send(void* content) {
    char* req = (char*)content;
    int sock = create_socket(hostname, port);
    CHECK(sock == -1, "sock create failed");
    
    size_t size = write(sock, req, sizeof(req));
    CHECK(size <= 0, "socket write error");
    return ;
}

void build_request(char* uri) {
    char tmp[10];
    int i;
    
    memset(request, 0, sizeof(request));
    
    switch (method) {
        default:
        case METHOD_GET:strcat(request, "GET");break;
        case METHOD_POST:strcat(request, "POST");break;
        case METHOD_HEAD:strcat(request, "HEAD");break;
    }
    
    strcat(request, " ");
    strcat(request, uri);
    
    //default HTTP version:1.1
    strcat(request, " HTTP/1.1");
    
    strcat(request, "\r\n");
    strcat(request, "User-Agent: Freesia"PROGRAM_VERSION"\r\n");
    strcat(request, "Host: ");
    strcat(request, uri);
    strcat(request, "\r\n");
    
    strcat(request, "Connection: close\r\n");
    strcat(request, "\r\n");
    
    printf("\nRequest:\n%s\n", request);
}
