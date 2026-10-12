#include <stdint.h>
#include "command.h"

#define COMMAND_MIN_LENGTH 4
#define BUFFER_SIZE 128

uint8_t readIndex=0,writeIndex=0;
uint8_t buffer[BUFFER_SIZE]={0};

static void addReadIndex(uint8_t length){
    readIndex=(readIndex+length)%BUFFER_SIZE;
}
static uint8_t readBuffer(uint8_t i){
    return buffer[i%BUFFER_SIZE];
}
static uint8_t bufferGetLength(){
    return (writeIndex-readIndex+BUFFER_SIZE)%BUFFER_SIZE;
}
static void appendArray(uint8_t *baseArray,uint8_t *appendArray,uint8_t appendPoint,uint8_t appendLength){
    for(uint8_t i=0;i<appendLength;i++){
        baseArray[appendPoint+i]=appendArray[i];
    }
    return;
}
static uint8_t checkDataPack(){
    if(readBuffer(readIndex)!=0xAA){
        return 1;
    }
    if(bufferGetLength()<COMMAND_MIN_LENGTH){
        return 2;
    }
    uint8_t length=readBuffer(readIndex+1);
    if(bufferGetLength()<length){
        return 2;
    }
    uint8_t sum=0;
    for(int i=0;i<length-1;i++){
        sum+=readBuffer(readIndex+i);
    }
    if(sum!=readBuffer(readIndex+length-1)){
        return 1;
    }
    return length;
}

uint8_t commandWrite(uint8_t *data,uint8_t length){//将data中的数据搬入到buffer中
    if(length>(BUFFER_SIZE-bufferGetLength())){
        return 0;
    }
    if(writeIndex+length<BUFFER_SIZE){
        appendArray(buffer,data,writeIndex,length);
        writeIndex+=length;
        return 1;
    }
    else{
        uint8_t first_length=BUFFER_SIZE-writeIndex;
        appendArray(buffer,data,writeIndex,first_length);
        appendArray(buffer,data+first_length,0,length-first_length);
        writeIndex=length-first_length;
        return 1;
    }
}
uint8_t commandRead(uint8_t *targetArray){//读取buffer中合法的数据包并将其搬入targetArray中
    while (1) {
        uint8_t check=checkDataPack();
        if(check==1){
            addReadIndex(1);
            continue;
        }
        else if(check==2){
            return 0;
        }
        else{
            uint8_t length=readBuffer(readIndex+1);
            for(uint8_t i=0;i<length;i++){
                targetArray[i]=readBuffer(readIndex+i);
                buffer[readIndex+i]=0;
            }
            addReadIndex(length);
            return length;
        }
    }
}
