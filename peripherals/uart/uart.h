
#ifndef UART_H_
  #define UART_H_



  enum eUartSettings_Speed
  {
    eUartSettings_Speed_9600,
    eUartSettings_Speed_19200,
    eUartSettings_Speed_38400,
    eUartSettings_Speed_57600,
    eUartSettings_Speed_115200,
    eUartSettings_Speed_230400,
    eUartSettings_Speed_921600
  };

  enum eUartSettings_StopBits
  {
    eUartSettings_StopBits_1,
    eUartSettings_StopBits_0_5,
    eUartSettings_StopBits_2,
    eUartSettings_StopBits_1_5
  };

  enum eUartSettings_ParityBits
  {
    eUartSettings_ParityBits_Even,
    eUartSettings_ParityBits_Odd,
  };

  enum eUartSettings_ParityCtrl
  {
    eUartSettings_ParityCtrl_Disable,
    eUartSettings_ParityCtrl_Enable
  };

  enum eUartSettings_DataBit
  {
    eUartSettings_DataBits_8bit,
    eUartSettings_DataBits_9bit
  };


  struct sUartSettings
  {
    enum eUartSettings_Speed Speed;
    enum eUartSettings_StopBits StopBits;
    enum eUartSettings_ParityBits ParityBits;
    enum eUartSettings_ParityCtrl ParityCtrl;
    enum eUartSettings_DataBit DataBit;
  };


  struct sUART
  {
    void (*afterDMA)(void);
    void (*afterTx)(void);
    void (*afterRx)(void);
    unsigned short TxCounter;
    unsigned short TxCount;
    unsigned short RxCounter;
    unsigned short RxCount;
    unsigned char Protocol;
    unsigned char TxBuffer[260];
    unsigned char RxBuffer[260];
  };


  struct specialUART
  {
    unsigned short TxCounter;
    unsigned short TxCount;
    unsigned short RxCounter;
    unsigned short RxCount;
    unsigned char TxBuffer[1024];
    unsigned char RxBuffer[260];
  };


  extern struct specialUART  sUART1;

  
  void InitUART1(void);
  void SendUART1(void *Buffer, unsigned short Count);
  void USART1_IRQHandler(void);

  void InitUART3(void);
  void SendUART1(void *Buffer, unsigned short Count);
  void USART3_IRQHandler(void);
  
  void Init_USART3_RCC();
  void Init_USART3_GPIO();
  void Init_USART3_Interrupt();
  void Init_USART3(int baud);

#endif
