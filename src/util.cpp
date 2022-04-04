/*
 * @Author: Limer
 * @Date: 2022-03-29 13:53:23
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-29 21:41:25
 * @Description: 
 */
#include "util.h"
#include <stdio.h>
#include <stdlib.h>


void errif(bool condition, const char * msg){
    if(condition == true){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int setnonblocking(int fd){
    int old_opt = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, old_opt|O_NONBLOCK);
    return old_opt;
}