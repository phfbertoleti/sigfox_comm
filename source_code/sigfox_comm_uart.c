/* Program: WSSFM11R2D SigFox modem communication Tool
*  Description: UART interaction module
 * Author: Pedro Bertoleti
*/

#include "sigfox_comm_uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */

/* Prototypes */
int open_and_prepare_uart(char * ptr_uart_path, int * file_descriptor);

/* Function: open and prepare UART for sending and receiving data in UART configuration needed (9600 8N1)
 * Parameters: pointer to UART file and pointer to UART file descriptor
 * Return: sucess (UART_PREPARE_SUCESS) or errors
*/
int open_and_prepare_uart(char * ptr_uart_path, int * file_descriptor)
{
    struct termios options;
    int result = 0;
    int function_return = UART_OPEN_COMM_ERROR;

    /* Open file */
    *file_descriptor = open(ptr_uart_path, O_RDWR | O_NOCTTY);
    if (*file_descriptor == -1)
    {
        printf("[ERROR] Impossible to open UART file.");
        function_return = UART_OPEN_COMM_ERROR;
        goto END_UART_PREPARE;
    }

    /* Flush away any bytes previously read or written */
    result = tcflush(*file_descriptor, TCIOFLUSH);
    if (result)
    {
        printf("[ERROR] Cannot flush UART file.");
        function_return =  UART_FLUSH_ERROR;
        goto END_UART_PREPARE;
    }

    /* Get the current configuration of the serial port */
    result = tcgetattr(*file_descriptor, &options);
    if (result)
    {
        printf("[ERROR] Cannot get current UART config / options.");
        function_return = UART_GET_CONFIG_ERROR;
        goto END_UART_PREPARE;
    }

    /* Turn off any options that might interfere with our ability to send and
      receive raw binary bytes */
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
 
    /* Set up timeouts: Calls to read() will return as soon as there is
       at least one byte available or when 100 ms has passed */
    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 0;

    /* SigFox modem default baudrate (read and write) is 9600 */
    cfsetospeed(&options, B9600);
    cfsetispeed(&options, cfgetospeed(&options));
 
    /* Write UART configs */
    result = tcsetattr(*file_descriptor, TCSANOW, &options);
   
    if (result)
    {
        printf("[ERROR] Cannot set UART config / options.");
        function_return = UART_SET_CONFIG_ERROR;        
        goto END_UART_PREPARE;
    }
    else
    {
        function_return = UART_PREPARE_SUCESS;
    }
    
END_UART_PREPARE:
    return function_return;    
}

/* Function: send data to SigFox Modem via UART
 * Parameters: pointer to UART file path; pointer to data to be sent; size of data to be sent
 * Return: sucess (UART_COMM_SUCESS) or errors
*/
int sigfox_comm_uart_send_data(char * ptr_uart_path, char * ptr_data, int data_size)
{
    int function_return = UART_SEND_DATA_ERROR;
    int fd = 0;
    ssize_t bytes_written_to_uart = 0;

    if (open_and_prepare_uart(ptr_uart_path ,&fd) != UART_PREPARE_SUCESS)
        goto END_UART_SEND;

    /* Now, UART is set up. Time to write data to it */
    bytes_written_to_uart = write(fd, ptr_data, data_size);
    if (bytes_written_to_uart != (ssize_t)data_size)
    {
        printf("[ERROR] Cannot write data to UART.");
        function_return = UART_SEND_DATA_ERROR;
        goto END_UART_SEND;
    }
    else
    {
        function_return = UART_COMM_SUCESS;
    }

END_UART_SEND:
    close(fd);
    return function_return;
}

/* Function: receive data from SigFox Modem via UART
 * Parameters: pointer to buffer to store data received; size of received data 
 * Return: sucess (UART_COMM_SUCESS) or errors (UART_OPEN_COMM_ERROR or UART_RCV_DATA_ERROR) 
*/
int sigfox_comm_uart_rcv_data(char * ptr_uart_path, char * ptr_data, int * ptr_data_rcv_size)
{
    int function_return = UART_RCV_DATA_ERROR;
    int fd = 0;
    size_t bytes_received_from_uart = 0;

    if (open_and_prepare_uart(ptr_uart_path ,&fd) != UART_PREPARE_SUCESS)
        goto END_UART_READ;

    /* Now, UART is set up. Time to read data from it */
    *ptr_data_rcv_size = 0;
    do
    {
        bytes_received_from_uart = read(fd, ptr_data, 1);
        if (bytes_received_from_uart > 0)
        {
            ptr_data++;
            *ptr_data_rcv_size++;
        }
        else
        {
            if (bytes_received_from_uart < 0)
            {
                printf("[ERROR] Error when reading data from UART");
                function_return = UART_RCV_DATA_ERROR;        
                break;
            }
        }        
    } while (bytes_received_from_uart > 0);    

    function_return = UART_COMM_SUCESS; 

END_UART_READ:
    close(fd);
    return function_return;
}