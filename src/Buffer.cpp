/*
 * @Author: Limer
 * @Date: 2022-04-11 13:19:48
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-15 19:23:10
 * @Description:
 */
#include "Buffer.h"
#include <iostream>

void Buffer::append(const char* _str, int _size) { buf.append(_str, _size); }

const char* Buffer::c_str() { return buf.c_str(); }

void Buffer::clear() { buf.clear(); }

ssize_t Buffer::size() { return buf.size(); }

void Buffer::getline() {
    buf.clear();
    std::getline(std::cin, buf);
}
