#include "StaticBuffer.h"

unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];

StaticBuffer::StaticBuffer(){
      for(int i = 0 ; i<BUFFER_CAPACITY ;i++){
              metainfo[i].free = true ;
      }
}
StaticBuffer::~StaticBuffer(){}

int StaticBuffer::getFreeBuffer(int blockNum){

        if(blockNum < 0 ||  blockNum> DISK_BLOCKS){
               return E_OUTOFBOUND ;
        }
        int alloctedbuffer ;

        for(int i = 0; i<BUFFER_CAPACITY ; i++){
              if(metainfo[i].free){
                   alloctedbuffer = i ;
                   break;
              }
        }
        metainfo[alloctedbuffer].free = false ;
        metainfo[alloctedbuffer].blockNum = blockNum ;

        return alloctedbuffer ;
}
int StaticBuffer::getBufferNum(int blockNum){
       if(blockNum < 0 || blockNum > DISK_BLOCKS){
              return E_OUTOFBOUND ;
       }
     for(int i = 0 ; i<BUFFER_CAPACITY;i++){
             if(metainfo[i].blockNum == blockNum){
                  return i ;
             }
     }

     return E_BLOCKNOTINBUFFER ;
}