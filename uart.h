#ifndef UART_H
#define UART_H




#define UART2_RX_BUFF_SIZE 0xc8            // 32 byte buffer

extern void InitialiseUART(void);
extern void UARTPrintF(byte *message, byte len);
extern byte src[200];
extern byte length;


//extern byte uart2_RX_data[UART2_RX_BUFF_SIZE]; ///< The receive buffer
extern byte uart2_RX_data[200]; ///< The receive buffer
extern byte uart2_RX_in; ///< Index to the next free location in the receive buffer
extern byte uart2_RX_out; ///< Index to the next unprocessed location in rx buffer
extern byte uart2_RX_parsing; ///< Index up to where the processing routine has gotten
extern void UART_RECEIVE_Churn (void);
extern void Uart_buffer_reset (byte t);



#endif