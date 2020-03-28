/* Program: WSSFM11R2D SigFox modem communication Tool
*  Description: AT commands formatting module
 * Author: Pedro Bertoleti
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sigfox_comm_format_at_cmds.h"

/* AT Commands definitions */
#define AT_CMD_CHECK_MICRO_MACRO_CHANNELS    "AT$GI?\r"
#define AT_CMD_RESET_SIGFOX_RADIO            "AT$RC\r"
#define AT_CMD_SEND_FRAME                    "AT$SF="


/* Function: format at command to send data to SigFox Cloud
 * Parameters: - pointer to data array to be sent
 *             - pointer to buffer to store formatted at command
 * Return: none
*/
void sigfox_comm_format_at_send_data(char * ptr_data_array, char * formatted_at_cmd)
{
    sprintf(formatted_at_cmd,"%s%s\r", AT_CMD_SEND_FRAME, ptr_data_array);
}

/* Function: format at command to check macro channels
 * Parameters: - pointer to buffer to store formatted at command
 *             - size of formatted at command
 * Return: none
*/
void sigfox_comm_format_at_macro_channel(char * formatted_at_cmd)
{
    sprintf(formatted_at_cmd, "%s", AT_CMD_CHECK_MICRO_MACRO_CHANNELS);
}

/* Function: format at command to reset SigFox radio and restore micro and macro channels
 * Parameters: - pointer to buffer to store formatted at command
 *             - size of formatted at command
 * Return: none
*/
void sigfox_comm_format_at_reset_radio(char * formatted_at_cmd)
{
    sprintf(formatted_at_cmd, "%s", AT_CMD_RESET_SIGFOX_RADIO);
}

