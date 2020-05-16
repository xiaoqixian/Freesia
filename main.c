/*
 * (C) Radim Kolar 1997-2004
 * 
 * Server concurrent test program, use threadpool
 */

#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#include "socket.h"
#include "threadpool.h"
#include "debug.h"

#define PROGRAM_VERSION "1.5"
#define PROGRAM_NAME "FREESIA"
#define REQUEST_SIZE 2048
#define MAX_HOST_LEN 100
#define METHOD_GET 1
#define METHOD_POST 2
#define METHOD_HEAD 3

char request[REQUEST_SIZE];
char host[MAX_HOST_LEN];
int method = 1;
char* hostname;
int port;
int thread_num = 10;
int clients = 10;
int failed = 0;
int success = 0;

typedef void (*func_ptr)(void* content);

void usage() {
    fprintf(stderr, "options\n"
            "-a | set ip address, required\n"
            "-p | set connect port, required\n"
            "-c | set the number clients to send request, optional, default 10\n"
            "-m | set request method, optional, default GET\n"
            "-n | set thread num, optional, default 10\n");
}

void send_request(void* content);
void build_request(char* uri);

int main(int argc, char* argv[]) {
    if (argc == 1) {
        usage();
        return -1;
    }
    

    int opt;
    char* op_string = "a:p:n::c::m::h";
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
            case 'c':
                clients = atoi(optarg);
                break;
            case 'm':
                method = atoi(optarg);
                break;
            case 'h':
                usage();
                return 0;
        }
    }
    
    //create threadpool
    threadpool_t* tp = NULL;
    tp = threadpool_init(thread_num);
    CHECK(tp == NULL, "threadpool init error");
    
    //build a request, shared by all threads
    build_request(hostname);
    
    func_ptr work = send_request;
    int i, res;
    for (i = 0; i < clients; i++) {
        res = threadpool_add(tp, work, request);
        if (res == -1) {
            LOG_ERR("threadpool add task error");
            continue;
        }
    }
    if (threadpool_destroy(tp, 1) < 0) {
        LOG_ERR("destroy threadpool failed");
    }
    printf("success: %d failed: %d\n", success, failed);
    return 0;
}

void send_request(void* content) {
    LOG_INFO("send request task");
    char* req = (char*)content;
    int sock = create_socket(hostname, port);
    if (sock == -1) {
        LOG_ERR("create socket error");
        failed++;
        return ;
    }
    
    size_t req_len = strlen(req);
    
    if (req_len != write(sock, req, req_len)) {
        failed++;
    }
    success++;
    if (close(sock) == -1) {
        LOG_ERR("socket close error");
        exit(1);
    }
}


void build_request(char* uri) {
    memset(request, 0, sizeof(request));
    
    switch (method) {
        default:
        case METHOD_GET:strcat(request, "GET");break;
        case METHOD_POST:strcat(request, "POST");break;
        case METHOD_HEAD:strcat(request, "HEAD");break;
    }
    
    strcat(request, " ");
    strcat(request, "/");
    
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
