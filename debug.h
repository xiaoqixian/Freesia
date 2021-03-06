/*
 * FILE: debug.h
 * 
 * debug some results with macro functions
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define CLEAN_ERRNO() (errno == 0? "None" : strerror(errno))

#define LOG_ERR(M, ...) fprintf(stderr, "[ERROR: %s:%d: errno: %s]" M "\n", __FILE__, __LINE__, CLEAN_ERRNO(), ##__VA_ARGS__)

#define CHECK(A, M, ...) if ((A)) {LOG_ERR(M "\n", ##__VA_ARGS__ );} 

#define CHECK_EXIT(A, M, ...) if ((A)) {LOG_ERR(M "\n", ##__VA_ARGS__ );exit(1);} 

#define LOG_INFO(M, ...) fprintf(stderr, "[INFO] " M "\n", ##__VA_ARGS__)

#endif
