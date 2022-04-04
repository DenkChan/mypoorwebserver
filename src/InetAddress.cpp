/*
 * @Author: Limer
 * @Date: 2022-03-29 19:09:56
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-29 20:57:36
 * @Description:
 */
#include "InetAddress.h"
#include <string.h>
#include <sys/socket.h>
InetAddress::InetAddress(){
  _addr_len = sizeof(_addr);
  bzero(&_addr,_addr_len);
}
InetAddress::InetAddress(const char *IP, int port) {
  _addr_len = sizeof(_addr);
  bzero(&_addr, sizeof(_addr));
  _addr.sin_addr.s_addr = inet_addr(IP);
  _addr.sin_port = htons(port);
  _addr.sin_family = PF_INET;
}