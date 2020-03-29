/* Program: WSSFM11R2D SigFox modem communication Tool
 * Arguments: first: UART path to modem communication (ex: /dev/ttyO4)
 *            third: this argument is the data array to be sent, hex-string format. 
 *                   IMPORTANT: this data array should have 24 chars as max size ( = 12 bytes in hex format)
 *                              if it's longer than this, it'll be truncated to 24 bytes.   
 * Author: Pedro Bertoleti
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sigfox_comm_uart.h"
#include "sigfox_comm_format_at_cmds.h"

/* Arguments definitions */
#define UART_ARG_IDX      1 
#define DATA_ARD_IDX      2
#define MIN_ARGS_NUMBER   2

/* Actions definitions */
#define ACTION_SEND_DATA        's'

/* Data definitions */
#define DATA_ARRAY_MAX_SIZE        24  // 24 bytes hex-string  = 12 bytes raw hex data
#define DATA_SIGFOX_MODEM_RETURN   200
#define DATA_ARRAY_FULL_AT_CMD     50

/* Macro channels definitions */
#define MACRO_CHANNEL_OK              0
#define MACRO_CHANNEL_FAIL           -1
#define MIN_MICRO_CHANNEL_AVAILABLE   3

/* Global variables  */
char uart_file[20] = {0}; 

/* Prototypes */
int check_macro_channels(void);


/* Function: check if needed macro channels are available
 * Parameters: none             
 * Return: sucess (MACRO_CHANNEL_OK) or failure (MACRO_CHANNEL_FAIL)
*/
int check_macro_channels(void)
{
    char macro_channel_buffer[DATA_ARRAY_FULL_AT_CMD] = {0};
    char macro_channel_response_buffer[DATA_ARRAY_FULL_AT_CMD] = {0};
    char * ptr_comma = NULL;
    char * ptr_micro_channel_number = NULL;
    char * ptr_macro_channel_number = NULL;
    int uart_operation_result = 0;
    int macro_channel_return = MACRO_CHANNEL_FAIL;
    int macro_channel_number = 0;
    int micro_channel_number = 0;

    sigfox_comm_format_at_macro_channel(macro_channel_buffer);
    uart_operation_result = sigfox_comm_uart_send_data(uart_file, macro_channel_buffer, strlen(macro_channel_buffer) );

    /* Checking macro channel command sent status */
    if (uart_operation_result != UART_COMM_SUCESS)
    {
        printf("[ERROR] Error when sending macro channel at command. Error code: %d\n", uart_operation_result); 
        macro_channel_return = MACRO_CHANNEL_FAIL;
        goto END_MACRO_CHANNEL;
    } 

    /* Checking available macro channels */ 
    uart_operation_result = sigfox_comm_uart_rcv_data(uart_file, macro_channel_response_buffer);
    if (uart_operation_result != UART_COMM_SUCESS)
    {
        printf("[ERROR] Error when receiving macro channel at command response. Error code: %d\n", uart_operation_result); 
        macro_channel_return = MACRO_CHANNEL_FAIL;
        goto END_MACRO_CHANNEL;
    }

    /*  get micro and macro channels numbers */
    ptr_comma = strchr(macro_channel_response_buffer, ',');

    if (ptr_comma != NULL)
    {
        ptr_micro_channel_number = ptr_comma - 1;        
        macro_channel_number = (*ptr_micro_channel_number) - 0x30;
        printf("Micro channel number: %d\n", macro_channel_number);

        ptr_macro_channel_number = ptr_comma + 1;
        macro_channel_number = (*ptr_macro_channel_number) - 0x30;
        printf("Macro channel number: %d\n", macro_channel_number);
    }
    else
    {
        printf("[ERROR] Error when parsing macro channel at command response\n"); 
        macro_channel_return = MACRO_CHANNEL_FAIL;
        goto END_MACRO_CHANNEL;
    }
    
    /* check if its needed to reset Sigfox radio to restore macro and micro channels numbers */
    if ( (macro_channel_number == 0) || (micro_channel_number < MIN_MICRO_CHANNEL_AVAILABLE) )
    {
        printf("[WARNING] Due to Micro and macro channels numbers acquired, the following action needed: reset SigFox radio to restore micro and macro channels\n");

        /* Reset SigFox radio */
        sigfox_comm_format_at_reset_radio(macro_channel_buffer);
        uart_operation_result = sigfox_comm_uart_send_data(uart_file, macro_channel_buffer, strlen(macro_channel_buffer));

        if (uart_operation_result != UART_COMM_SUCESS)
        {
            printf("[ERROR] Error when sending reset SigFox radio at command. Error code: %d\n", uart_operation_result); 
            macro_channel_return = MACRO_CHANNEL_FAIL;
            goto END_MACRO_CHANNEL;
        }     
        else
            macro_channel_return = MACRO_CHANNEL_OK;        
    }

END_MACRO_CHANNEL:
    return macro_channel_return;                                                     
}

/* 
 * Main function 
 */

int main (int argc, char *argv[])
{
    int valid_arguments = 0;
    int data_array_size = 0;
    int data_array_size_informed = 0;
    int uart_operation_result = 0;
    char array_data_size[DATA_ARRAY_MAX_SIZE+1] = {0}; //24 bytes + '\r' char
    char array_full_at_command[DATA_ARRAY_FULL_AT_CMD] = {0};

    /* parse aguments */
    valid_arguments = argc-1;
    
    if (valid_arguments < MIN_ARGS_NUMBER)
    {
        printf("\n\n[ERROR] Insuficient arguments. Please, review your command and try again.\n");
        exit(-1);
    }

    memcpy(uart_file, argv[UART_ARG_IDX], strlen(argv[UART_ARG_IDX]) );

   
    /* Send data */
    printf("\n\nSigFox WSSFM11R2D data send"); 

    data_array_size_informed = strlen(argv[DATA_ARD_IDX]);
    if (data_array_size_informed > DATA_ARRAY_MAX_SIZE)
        data_array_size = DATA_ARRAY_MAX_SIZE;
    else
        data_array_size = data_array_size_informed;     

    memcpy(array_data_size, argv[DATA_ARD_IDX], data_array_size);   

    printf("\n\nChecking macro channels...\n");
    if (check_macro_channels() != MACRO_CHANNEL_OK)
    {
        printf("[ERROR] Impossible to check or ensure enough macro channels\n"); 
        exit(-1); 
    }

    printf("\n\nData to send: %s\nSending data...\n", array_data_size);
    sigfox_comm_format_at_send_data(array_data_size, array_full_at_command);                
    uart_operation_result = sigfox_comm_uart_send_data(uart_file, array_full_at_command, strlen(array_full_at_command));

    /* Sending data to SigFox modem */
    if (uart_operation_result != UART_COMM_SUCESS)
    {
        printf("[ERROR] Error when sending data. Error code: %d\n", uart_operation_result); 
        exit(-1);
    }                    

    printf("Data sent successfully!\n\n");
    return 0;
}
