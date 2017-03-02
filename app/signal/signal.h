//---------------------------------------------------
//                   signal.h
//---------------------------------------------------

#ifndef SIGNAL_H_
#define SIGNAL_H_


#define SIGNALGEN_STATUS_IDLE       0
#define SIGNALGEN_STATUS_GENERATE   1

#define DEV_DIRECT_UP     0
#define DEV_DIRECT_DOWN   1

extern struct sSignalGen SignalGen;

#define SIGNAL_OFF      0
#define SIGNAL_KNOWING  3
#define SIGNAL_WARNING_NR 2

struct sSignalGen
{
  float DevMin;
  float Dev;
  float DevStep; // dewiacja krok
  float DevMax;
  unsigned char SignalNr;
  unsigned char Status;
  unsigned char Step;
  unsigned char Signal_Set; // zadany sygnał
  unsigned char DevDirect; // kierunek odstrajania
  unsigned char PacketCounter;
};

enum eSignalType
{
  eSignalType_NoneType,
  eSignalType_Information,
  eSignalType_Warning,
  eSignalType_Starting,
};

void InitExternalKSR(void);
void SignalGen_Handler(void);
void SignalGen_SetSignal(unsigned char Nr);
void SignalGen_ExternDev_Handler(void);
void SignalGen_ExternDev_Set(unsigned char SignalNr);
unsigned char SignalGenExt_Check(void);
void SignalGenExt_StartSignal(void);
unsigned char SignalGenExt_GetSignalNr(void);
enum eSignalType SignalGen_TypeSignal(void);

#endif
