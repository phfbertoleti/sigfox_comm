/* Program: WSSFM11R2D SigFox modem communication Tool
*  Description: AT commands formatting module header
 * Author: Pedro Bertoleti
*/

#ifndef SIGFOX_COMM_AT_CMD_FORMAT
#define SIGFOX_COMM_AT_CMD_FORMAT

/* Prototypes */
void sigfox_comm_format_at_wake_up(char * formatted_at_cmd);
void sigfox_comm_format_at_send_data(char * ptr_data_array, char * formatted_at_cmd);
void sigfox_comm_format_at_macro_channel(char * formatted_at_cmd);
void sigfox_comm_format_at_reset_radio(char * formatted_at_cmd);

#endif