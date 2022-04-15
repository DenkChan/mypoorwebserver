/*
 * @Author: Limer
 * @Date: 2022-04-11 13:16:19
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-15 17:00:53
 * @Description:
 */
#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <string>
class Buffer {
   private:
    std::string buf;

   public:
    void append(const char*, int);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
};
#endif