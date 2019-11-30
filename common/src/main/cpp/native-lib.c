#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include "http.h"
#include <android/log.h>
#include "data2.h"

#define LOG_TAG  "C_TAG"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define BUFFER_SIZE 1024
#define HTTP_POST "POST /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n"\
    "Content-Type:application/x-www-form-urlencoded\r\nContent-Length: %zu%s\r\n\r\n%s"
#define HTTP_GET "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*%s\r\n\r\n"
//static const char *BASE_URL = "http://172.17.8.100/small/";
static const char *BASE_URL = "http://mobile.bwstudent.com/small/";

int http_tcpclient_create(const char *host, int port) {
    struct hostent *he;
    struct sockaddr_in server_addr;
    int socket_fd;

    if ((he = gethostbyname(host)) == NULL) {
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *) he->h_addr);

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOGD("C-HTTP-socket创建失败1");
        return -1;
    }

    if (connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
        LOGD("C-HTTP-socket创建失败2");
        return -1;
    }
    LOGD("C-HTTP-socket创建成功");
    return socket_fd;
}

/*
 * 关闭连接
 * */
void http_tcpclient_close(int socket) {
    close(socket);
}

/*
 * 解析URL
 * */
int http_parse_url(const char *url, char *host, char *file, int *port) {
    char *ptr1, *ptr2;
    int len = 0;
    if (!url || !host || !file || !port) {
        return -1;
    }

    ptr1 = (char *) url;

    if (!strncmp(ptr1, "http://", strlen("http://"))) {
        ptr1 += strlen("http://");
    } else {
        return -1;
    }

    ptr2 = strchr(ptr1, '/');
    if (ptr2) {
        len = strlen(ptr1) - strlen(ptr2);
        memcpy(host, ptr1, len);
        host[len] = '\0';
        if (*(ptr2 + 1)) {
            memcpy(file, ptr2 + 1, strlen(ptr2) - 1);
            file[strlen(ptr2) - 1] = '\0';
        }
    } else {
        memcpy(host, ptr1, strlen(ptr1));
        host[strlen(ptr1)] = '\0';
    }
    //get host and ip
    ptr1 = strchr(host, ':');
    if (ptr1) {
        *ptr1++ = '\0';
        *port = atoi(ptr1);
    } else {
        *port = MY_HTTP_DEFAULT_PORT;
    }

    return 0;
}


char *http_tcpclient_recv(int socket) {
    int recvnum;
    int length = 0;
    char *data;
    do {
        char d[BUFFER_SIZE] = {'\0'};
        LOGD("哈哈哈哈");
        recvnum = recv(socket, d, BUFFER_SIZE, 0);
        length += recvnum;
        LOGD("哈哈哈哈1");
        char *a = (char *) malloc(strlen(data) + strlen(d) + 1);

        strcpy(a, data);
        sprintf(data, "%s%s", data, d);
        LOGD("socket本次读取数据长度为：%d，总长度为：%d----本次数据为：%s", recvnum, length, d);
    } while (recvnum == BUFFER_SIZE);
    return data;
}

//自定义Read函数
//参数一：自己程序的socket
//返回值：成功返回指向存储有相应内容的动态内存的指针，失败返回NULL
//注意：1）返回的动态内存指针在不使用的时候应该通过free释放；2）如果不是真的有问题，请不要动，如果读取数据出现问题，请优先检查data2.c中的函数
char *Read(int socket) {
    int length = 0, return_length;
    char buffer[1024];
    char *Data;
    PBUFFER header, nowBuffer;//nowBuffer指向正在使用的BUFFER节点


    if (NULL != (header = create_EmptyBufferLink()))//创建BUFFER表头
    {
        if (NULL == (nowBuffer = append_Buffer_Node(header)))//创建第一个存储响应的BUFFER节点
        {
            LOGD("\nappend_Buffer_Node() fail in http.c Read()\n");//节点添加失败直接返回
            free_Buffer_Link(header);
            return NULL;
        }

    } else {
        LOGD("\ncreate_EmptyBufferLink() fail in http.c Read()\n");//头结点创建失败直接返回
        return NULL;
    }

    //每次读取1024个节点存储到buffer中，然后再通过strncpy复制到BUFFER节点当中
    while ((return_length = read(socket, buffer, 1024)) > 0) {
        if (return_length == -1) {
            LOGD("\nreceive wrong!\n");
            free_Buffer_Link(header);
            header = NULL;
            return NULL;
        } else {

            if (length >= 50176)//如果节点已经快要存满，则新建节点，将相应内容存到新建立的节点当中
            {
                nowBuffer->data[length] = '\0';
                if (NULL == (nowBuffer = append_Buffer_Node(header))) {
                    LOGD("\nappend_Buffer_Node() fail in http.c Read()\n");//节点添加失败直接返回
                    free_Buffer_Link(header);
                    return NULL;
                }
                length = 0;
                strncpy(nowBuffer->data + length, buffer, return_length);
                length += return_length;
            } else {
                strncpy(nowBuffer->data + length, buffer, return_length);
                length += return_length;
            }
        }

    }

    nowBuffer->data[length] = '\0';
    Data = get_All_Buffer(header);//将BUFFER链表中的内容取出，存储到动态内存当中

    //释放BUFFER链表
    if (header != NULL) {
        free_Buffer_Link(header);
        header = NULL;
    }

    if (length == 0) {
        LOGD("no date receive!\n");
        return NULL;
    }

    return Data;//返回指向存储有响应内容的动态内存的指针(可能为空)
}

static int http_tcpclient_send(int socket, char *buff, int size) {
    int sent = 0, tmpres = 0;

    while (sent < size) {
        tmpres = send(socket, buff + sent, size - sent, 0);
        if (tmpres == -1) {
            return -1;
        }
        sent += tmpres;
    }
    return sent;
}

static char *http_parse_result(const char *lpbuf) {
    char *ptmp = NULL;
    char *response = NULL;
    ptmp = (char *) strstr(lpbuf, "HTTP/1.1");
    if (!ptmp) {
        printf("http/1.1 not faind\n");
        return NULL;
    }
    if (atoi(ptmp + 9) != 200) {
        printf("result:\n%s\n", lpbuf);
        return NULL;
    }

    ptmp = (char *) strstr(lpbuf, "\r\n\r\n");
    if (!ptmp) {
        printf("ptmp is NULL\n");
        return NULL;
    }
    response = (char *) malloc(strlen(ptmp) + 1);
    if (!response) {
        printf("malloc failed \n");
        return NULL;
    }
    strcpy(response, ptmp + 4);
    return response;
}

/*
 * Post请求
 * */
char *http_post(const char *url, const char *headers, const char *post_str) {

    char post[BUFFER_SIZE] = {'\0'};
    int socket_fd = -1;
    char lpbuf[BUFFER_SIZE * 4] = {'\0'};
    char *ptmp;
    char host_addr[BUFFER_SIZE] = {'\0'};
    char file[BUFFER_SIZE] = {'\0'};
    int port = 0;
    int len = 0;
    char *response = NULL;

    if (!url || !post_str) {
        printf("      failed!\n");
        return NULL;
    }

    if (http_parse_url(url, host_addr, file, &port)) {
        printf("http_parse_url failed!\n");
        return NULL;
    }
    //printf("host_addr : %s\tfile:%s\t,%d\n",host_addr,file,port);
    LOGD("host_addr : %s\tfile:%s\t,%d\n", host_addr, file, port);

    socket_fd = http_tcpclient_create(host_addr, port);
    if (socket_fd < 0) {
        LOGD("http_tcpclient_create failed\n");
        return NULL;
    }

    sprintf(lpbuf, HTTP_POST, file, host_addr, port, strlen(post_str), headers, post_str);
    LOGD("%s", lpbuf);
    if (http_tcpclient_send(socket_fd, lpbuf, strlen(lpbuf)) < 0) {
        LOGD("http_tcpclient_send failed..\n");
        return NULL;
    }
    //printf("发送请求:\n%s\n",lpbuf);

    char *data = Read(socket_fd);
    LOGD("最终结果为：%s", data);
    http_tcpclient_close(socket_fd);

    return http_parse_result(data);
}

/*
 * Get请求
 * */
char *http_get(const char *url, const char *headers) {

    char post[BUFFER_SIZE] = {'\0'};
    int socket_fd = -1;
    char lpbuf[BUFFER_SIZE * 4] = {'\0'};
    char *ptmp;
    char host_addr[BUFFER_SIZE] = {'\0'};
    char file[BUFFER_SIZE] = {'\0'};
    int port = 0;
    int len = 0;

    if (!url) {
        LOGD("      failed!\n");
        return NULL;
    }

    if (http_parse_url(url, host_addr, file, &port)) {
        LOGD("http_parse_url failed!\n");
        return NULL;
    }
    //printf("host_addr : %s\tfile:%s\t,%d\n",host_addr,file,port);

    socket_fd = http_tcpclient_create(host_addr, port);
    if (socket_fd < 0) {
        LOGD("http_tcpclient_create failed\n");
        return NULL;
    }

    sprintf(lpbuf, HTTP_GET, file, host_addr, port, headers);
    LOGD("%s", lpbuf);

    if (http_tcpclient_send(socket_fd, lpbuf, strlen(lpbuf)) < 0) {
        LOGD("http_tcpclient_send failed..\n");
        return NULL;
    }
    //	printf("发送请求:\n%s\n",lpbuf);

    char *data = Read(socket_fd);
    LOGD("最终结果为：%s", data);
    http_tcpclient_close(socket_fd);

    return http_parse_result(data);
}

/*
 * 字符串转换
 * */
char *Jstring2CStr(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env, "GB2312");
//    jstring   strencode   =  (*env)->NewStringUTF(env,"UTF-8");
    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) (*env)->CallObjectMethod(env, jstr, mid, strencode);
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte *ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);         //new   char[alen+1];
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);
    if (rtn == NULL) {
        rtn = "";
    }
    return rtn;
}

JNIEXPORT jstring JNICALL
Java_com_dingtao_common_core_http_NetworkManager_postByJNI(
        JNIEnv *env,
        jobject jobj, jstring url, jstring headers, jstring params) {
    char *cu = Jstring2CStr(env, url);
    char *hu = Jstring2CStr(env, headers);
    char *pu = (char *) malloc(strlen(BASE_URL) + strlen(cu) + 1);
    strcpy(pu, BASE_URL);
    strcat(pu, cu);
    LOGD("Post_URL : %s", pu);
    LOGD("Post_Header : %s", hu);
    char *pm = Jstring2CStr(env, params);
    LOGD("Post_Param : %s", pm);
    char *result = http_post(pu, hu, pm);
    LOGD("Post_Result : %s", result);
    return (*env)->NewStringUTF(env, result);
}

JNIEXPORT jstring JNICALL
Java_com_dingtao_common_core_http_NetworkManager_getByJNI(
        JNIEnv *env,
        jobject thiz, jstring url, jstring headers) {
    char *cu = Jstring2CStr(env, url);
    char *hu = Jstring2CStr(env, headers);
    char *pu = (char *) malloc(strlen(BASE_URL) + strlen(cu) + 1);
    strcpy(pu, BASE_URL);
    strcat(pu, cu);
    LOGD("GET_URL=%s", pu);
    LOGD("GET_Header : %s", hu);
    char *result = http_get(pu, hu);
    LOGD("GET_Result=%s", result);
//    if(result==NULL||strlen(result)==0){
    return (*env)->NewStringUTF(env, result);
//    }
//    return (*env)->NewStringUTF(env, result);
}