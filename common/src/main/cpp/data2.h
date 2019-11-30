//
// Created by summer on 2019-12-01.
//

#ifndef textbuffer_h
#define textbuffer_h
struct textbuffer
{
    char data[52224];
    struct textbuffer *next;

};


typedef struct textbuffer TEXTBUFFER,*PBUFFER;

extern PBUFFER create_EmptyBufferLink();
extern PBUFFER create_EmptyBuffer();
extern PBUFFER append_Buffer_Node(PBUFFER header);
extern int free_Buffer_Link(PBUFFER header);
extern unsigned long count_Buffer_Node(PBUFFER header);
extern char* get_All_Buffer(PBUFFER header);
#endif // textbuffer_h
