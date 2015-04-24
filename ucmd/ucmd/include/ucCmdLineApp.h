#ifndef UCCMDLINEAPP_H
#define UCCMDLINEAPP_H

#include <stddef.h>
#include "ucCmdParser.h"
#include "ucCmdLineOpt.h"
#include "ucErr.h"

/*
 * Summary:
 *   An application that runs with a set of command options.
 */
typedef struct ucCmdLineApp ucCmdLineApp;

/*
 * Summary:
 *   The type of the function used by an application to
 *   receive data.
 * Parameters:
 *   buf: A string buffer that can be used to store the data received.
 *   buf_size: The size of the string buffer used to store received data.
 *   state: A stateful object.
 * Returns:
 *   The data that was received.
 */
typedef char *(ucCmdLineApp_ReceiveFunc)(char *buf, size_t buf_size, void *state);

/*
 * Summary:
 *   Sets the escape string that will cause the app to exit.
 * Parameters:
 *   value: The escape string that, when returned in a response,
 *          causes the app to exit.
 */
uc_EXPORTED void ucCmdLineApp_set_escape_response(ucCmdLineApp*, const char *value);

/*
 * Summary:
 *   Gets the escape string that causes the app to exit.
 * Returns:
 *   The escape string that, when returned in a response,
 *   causes the app to exit.
 */
uc_EXPORTED const char *ucCmdLineApp_get_escape_response(ucCmdLineApp*);

/*
 * Summary:
 *   Runs the application with the given options.
 * Parameters:
 *   cmd_opt: A pointer to the first command option for the app.
 * Returns:
 *   An error code, if one occurred.
 */ 
uc_EXPORTED ucErr ucCmdLineApp_run(ucCmdLineApp*, ucCmdLineOpt *cmd_opt);

/*
 * Summary:
 *   Gets a static, default instance of the application.
 * Returns:
 *   The static, default instance of the application.
 */
uc_EXPORTED ucCmdLineApp *ucCmdLineApp_get_instance(void);

/*
 * Summary:
 *   Sets the function that the app uses to receive data.
 * Parameters:
 *   value: A pointer to the function used to receive data.
 */
uc_EXPORTED void ucCmdLineApp_set_receive(ucCmdLineApp*, ucCmdLineApp_ReceiveFunc *value);

/*
 * Summary:
 *   Gets the function that the app uses to receive data.
 * Returns:
 *   A pointer to the function used to receive data.
 */ 
uc_EXPORTED ucCmdLineApp_ReceiveFunc *ucCmdLineApp_get_receive(ucCmdLineApp*);

/*
 * Summary:
 *   Gets the stateful object passed to the application's receive function.
 * Returns:
 *   A pointer to the stateful object passed to the application's receive function.
 */
uc_EXPORTED void *ucCmdLineApp_get_receive_state(ucCmdLineApp*);

/*
 * Summary:
 *   Sets the stateful object passed to the application's receive function.
 * Parameters:
 *   value: The stateful object passed to the application's receive function.
 */
uc_EXPORTED void ucCmdLineApp_set_receive_state(ucCmdLineApp*, void *value);

/*
 * Summary:
 *   Sets the command used to quit the application.
 * Parameters:
 *   value: The value of the command that quits the application.
 */
uc_EXPORTED void ucCmdLineApp_set_quit_command(ucCmdLineApp*, const char *value);

/*
 * Summary:
 *   Gets the value of the command that quits the application.
 * Returns:
 *   The value of the command that quits the application.
 */
uc_EXPORTED const char *ucCmdLineApp_get_quit_command(ucCmdLineApp*);

/*
 * Summary:
 *   Sets the value of the command that shows help information.
 * Parameters:
 *   value: The value of the command that shows help information.
 */
uc_EXPORTED void ucCmdLineApp_set_help_command(ucCmdLineApp*, const char *value);

/*
 * Summary:
 *   Gets the value of the command that shows help information.
 * Returns:
 *   The value of the command that shows help information.
 */
uc_EXPORTED const char *ucCmdLineApp_get_help_command(ucCmdLineApp*);

/*
 * Summary:
 *   Sets the command structure that the application uses.
 * Parameters:
 *   value: The command structure to be used by the application.
 */
uc_EXPORTED void ucCmdLineApp_set_cmd(ucCmdLineApp*, ucCmdLine *value);

/*
 * Summary:
 *   Gets the command structure used by the application.
 * Returns:
 *   A pointer to the command structure used by the application.
 */
uc_EXPORTED ucCmdLine *ucCmdLineApp_get_cmd(ucCmdLineApp*);

/*
 * Summary:
 *   Gets the command parser used by the application.
 * Returns:
 *   A pointer to the command parser used by the application.
 */
uc_EXPORTED ucCmdParser *ucCmdLineApp_get_cmd_parser(ucCmdLineApp*);

#endif
