


#ifndef CRC_H_
#define CRC_H_


void InitCRC32Module(void);
unsigned char CheckCRC16(void *PtrToData, unsigned char DataCount, unsigned short StartValue);
unsigned short GetCRC16(void *PtrToData, unsigned char DataCount, unsigned short StartValue);
unsigned int GetCRC32(void *PtrToData, unsigned int DataCount);





#endif
