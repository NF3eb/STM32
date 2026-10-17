#ifndef __COMMAND_H__
#define __COMMAND_H__

uint8_t commandWrite(const uint8_t *data,uint8_t length);//将data中的数据搬入到buffer中
uint8_t commandRead(uint8_t *targetArray);//读取buffer中合法的数据包并将其搬入targetArray中

#endif