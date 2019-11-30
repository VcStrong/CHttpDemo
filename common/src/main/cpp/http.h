//
// Created by Administrator on 2019/7/30.
//

#ifndef _MY_HTTP_H
#define _MY_HTTP_H
#define MY_HTTP_DEFAULT_PORT 80

char* http_get(const char* url, const char *headers);
char* http_post(const char* url, const char *headers, const char* post_str);
#endif //_MY_HTTP_H
