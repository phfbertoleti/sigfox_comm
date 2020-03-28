/* Program: WSSFM11R2D SigFox modem communication Tool
*  Description: UART interaction module header
 * Author: Pedro Bertoleti
*/

#ifndef SIGFOX_COMM_UART_DEF
#define SIGFOX_COMM_UART_DEF

/* UART returns definitions */
#define UART_PREPARE_SUCESS      0
#define UART_COMM_SUCESS         0
#define UART_OPEN_COMM_ERROR    -1
#define UART_FLUSH_ERROR        -2
#define UART_GET_CONFIG_ERROR   -3
#define UART_SET_CONFIG_ERROR   -4
#define UART_SEND_DATA_ERROR    -5
#define UART_RCV_DATA_ERROR     -6 

/* Prototypes */ 
int sigfox_comm_uart_send_data(char * ptr_uart_path, char * ptr_data, int data_size);
int sigfox_comm_uart_rcv_data(char * ptr_uart_path, char * ptr_data, int * ptr_data_rcv_size);

#endif