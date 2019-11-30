//
// Created by summer on 2019-12-01.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//定义节点结构体用于缓存HTTP响应
//textbuffer_h表示已经定义了textbuffer结构体（防止重复定义）
#define textbuffer_h
struct textbuffer
{
    char data[52224];
    struct textbuffer *next;

};
typedef struct textbuffer TEXTBUFFER,*PBUFFER;


//创建头节点
//返回值：成功返回指向动态内存指针，失败返回NULL
//注：1)创建头结点之后，注意检查头结点是否为空再使用;2)头结点的下一个结点才开始存储内容
PBUFFER create_EmptyBufferLink()
{

    PBUFFER header;
    header=(PBUFFER)malloc(sizeof(TEXTBUFFER));


    if(header!=NULL)
    {
        memset(header,0,sizeof(TEXTBUFFER));
        header->next=NULL;
    }

    return header;

}

//创建一个空的节点
//如果动态分配成功，局部变量node当中存有的是动态内存的地址，但是为防止极端情况（有人故意将可分配的动态空间占用完），还是改了一下代码
//返回值：成功返回指向动态内存指针，失败返回NULL
PBUFFER create_EmptyBuffer()
{
    PBUFFER node;
    if(NULL!=(node=(PBUFFER)malloc(sizeof(TEXTBUFFER))))
    {
        memset(node,0,sizeof(TEXTBUFFER));
        node->next=NULL;
    }

    return node;

}

//向链表尾部添加节点，返回新添加节点的指针，节点的内容可以通过返回的节点指针向其添加
//注：注意检查新分配的节点是否为NULL
PBUFFER append_Buffer_Node(PBUFFER header)
{

    PBUFFER newNode,nowNode;
    if(header==NULL)
    {
        printf("header is null!\n");
        return 0;
    }
    newNode=create_EmptyBuffer();

    nowNode=header;

    while(nowNode->next!=NULL)
    {
        nowNode=nowNode->next;

    }

    nowNode->next=newNode;

    return newNode;
}

//清空除了头结点之外其他的所有节点
int empty_Buffer_Node(PBUFFER header)
{
    PBUFFER nowNode,freeNode;

    if(header==NULL)
    {
        printf("header is null!\n");
        return 0;
    }

    nowNode=header;
    nowNode=nowNode->next;
    while(nowNode!=NULL)
    {
        freeNode=nowNode;
        nowNode=nowNode->next;
        free(freeNode);
    }

    header->next=NULL;

    return 1;

}

//清空包括头结点在内的所有节点
int free_Buffer_Link(PBUFFER header)
{

    if(header==NULL)
    {
        printf("header is null!\n");
        return 0;
    }

    empty_Buffer_Node(header);
    free(header);
    header=NULL;
    return 1;
}

//计算BUFFER链表一共存储了多少字节的响应，并且返回最终结果
unsigned long count_Buffer_Node(PBUFFER header)
{
    PBUFFER nowNode;
    unsigned long i=0;
    if(header==NULL)
    {
        printf("header is null!\n");
        return 0;
    }

    nowNode=header;

    while(nowNode->next!=NULL)
    {
        nowNode=nowNode->next;
        i+=strlen(nowNode->data);
    }

    return i;
}

//将整个BUFFER链表的内容提取出来，并存储到动态内存之中，返回动态内存的指针
//注：1）执行到此步，若是不再使用BUFFER链表，应用int free_Buffer_Link(PBUFFER header)释放链表
//2)返回的动态内存指针，若是不再使用动态内存里面的内容应通过free将其释放
char* get_All_Buffer(PBUFFER header)
{
    unsigned long i;
    PBUFFER nowNode;
    char *result;
    if(header==NULL)
    {
        printf("header is null!\n");
        return NULL;
    }
    i=count_Buffer_Node(header);
    result=(char*)malloc((i+100)*sizeof(char));
    memset(result,'\0',i*sizeof(char));

    nowNode=header;

    while(nowNode->next!=NULL)
    {
        nowNode=nowNode->next;
        strcat(result,nowNode->data);
    }
    printf("\nresult is:%s\n",result);
    return result;

}