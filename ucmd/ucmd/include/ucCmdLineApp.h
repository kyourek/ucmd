#ifndef UCCMDLINEAPP_H
#define UCCMDLINEAPP_H

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
typedef char *(ucCmdLineApp_receive_func)(char *buf, size_t buf_size, void *state);

/*
 * Summary:
 *   Sets the escape string that will cause the app to exit.
 * Parameters:
 *   p: A pointer to the app whose property is set.
 *   value: The escape string that, when returned in a response,
 *          causes the app to exit.
 */
uc_EXPORTED void ucCmdLineApp_set_escape_response(ucCmdLineApp *p, const char *value);

/*
 * Summary:
 *   Gets the escape string that causes the app to exit.
 * Parameters:
 *   p: A pointer to the app whose property is returned.
 * Returns:
 *   The escape string that, when returned in a response,
 *   causes the app to exit.
 */
uc_EXPORTED const char *ucCmdLineApp_get_escape_response(ucCmdLineApp *p);

/*
 * Summary:
 *   Runs the application with the given options.
 * Parameters:
 *   p: A pointer to the app to run.
 *   cmd_opt: A pointer to the first command option for the app.
 * Returns:
 *   An error code, if one occurred.
 */ 
uc_EXPORTED ucErr ucCmdLineApp_run(ucCmdLineApp *p, ucCmdLineOpt *cmd_opt);

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
 *   p: A pointer to the app whose property is set.
 *   value: A pointer to the function used to receive data.
 */
uc_EXPORTED void ucCmdLineApp_set_receive(ucCmdLineApp *p, ucCmdLineApp_receive_func *value);

/*
 * Summary:
 *   Gets the function that the app uses to receive data.
 * Parameters:
 *   p: A pointer to the app whose property is returned.
 * Returns:
 *   A pointer to the function used to receive data.
 */ 
uc_EXPORTED ucCmdLineApp_receive_func *ucCmdLineApp_get_receive(ucCmdLineApp *p);

/*
 * Summary:
 *   Gets the stateful object passed to the application's receive function.
 * Parameters:
 *   p: A pointer to the application object whose property is to be returned.
 * Returns:
 *   A pointer to the stateful object passed to the application's receive function.
 */
uc_EXPORTED void *ucCmdLineApp_get_receive_state(ucCmdLineApp *p);

/*
 * Summary:
 *   Sets the stateful object passed to the application's receive function.
 * Parameters:
 *   p: A pointer to the application object whose property is to be set.
 *   value: The stateful object passed to the application's receive function.
 */
uc_EXPORTED void ucCmdLineApp_set_receive_state(ucCmdLineApp *p, void *value);

/*
 * Summary:
 *   Sets the command used to quit the application.
 * Parameters:
 *   p: A pointer to the application.
 *   value: The value of the command that quits the application.
 */
uc_EXPORTED void ucCmdLineApp_set_quit_command(ucCmdLineApp *p, const char *value);

/*
 * Summary:
 *   Gets the value of the command that quits the application.
 * Parameters:
 *   p: A pointer to the application.
 * Returns:
 *   The value of the command that quits the application.
 */
uc_EXPORTED const char *ucCmdLineApp_get_quit_command(ucCmdLineApp *p);

/*
 * Summary:
 *   Sets the value of the command that shows help information.
 * Parameters:
 *   p: A pointer to the application whose property is set.
 *   value: The value of the command that shows help information.
 */
uc_EXPORTED void ucCmdLineApp_set_help_command(ucCmdLineApp *p, const char *value);

/*
 * Summary:
 *   Gets the value of the command that shows help information.
 * Parameters:
 *   p: A pointer to the app whose property is returned.
 * Returns:
 *   The value of the command that shows help information.
 */
uc_EXPORTED const char *ucCmdLineApp_get_help_command(ucCmdLineApp *p);

/*
 * Summary:
 *   Sets the command structure that the application uses.
 * Parameters:
 *   p: A pointer to the application.
 *   value: The command structure to be used by the application.
 */
uc_EXPORTED void ucCmdLineApp_set_cmd(ucCmdLineApp *p, ucCmdLine *value);

/*
 * Summary:
 *   Gets the command structure used by the application.
 * Parameters:
 *   p: A pointer to the application.
 * Returns:
 *   A pointer to the command structure used by the application.
 */
uc_EXPORTED ucCmdLine *ucCmdLineApp_get_cmd(ucCmdLineApp *p);

/*
 * Summary:
 *   Gets the command parser used by the application.
 * Parameters:
 *   p: A pointer to the application.
 * Returns:
 *   A pointer to the command parser used by the application.
 */
uc_EXPORTED ucCmdParser *ucCmdLineApp_get_cmd_parser(ucCmdLineApp *p);

#endif
