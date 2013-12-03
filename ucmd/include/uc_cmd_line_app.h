#ifndef UC_CMD_LINE_APP_H
#define UC_CMD_LINE_APP_H

#include "uc_cmd_parser.h"
#include "uc_cmd_line_opt.h"
#include "uc_err.h"

/*
 * Summary:
 *   An application that runs with a set of command options.
 */
typedef struct uc_cmd_line_app uc_cmd_line_app;

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
typedef char *(uc_cmd_line_app_receive_func)(char *buf, size_t buf_size, void *state);

/*
 * Summary:
 *   Sets the escape string that will cause the app to exit.
 * Parameters:
 *   p: A pointer to the app whose property is set.
 *   value: The escape string that, when returned in a response,
 *          causes the app to exit.
 */
UC_EXPORTED void uc_cmd_line_app_set_escape_response(uc_cmd_line_app *p, const char *value);

/*
 * Summary:
 *   Gets the escape string that causes the app to exit.
 * Parameters:
 *   p: A pointer to the app whose property is returned.
 * Returns:
 *   The escape string that, when returned in a response,
 *   causes the app to exit.
 */
UC_EXPORTED const char *uc_cmd_line_app_get_escape_response(uc_cmd_line_app *p);

/*
 * Summary:
 *   Runs the application with the given options.
 * Parameters:
 *   p: A pointer to the app to run.
 *   cmd_opt: A pointer to the first command option for the app.
 * Returns:
 *   An error code, if one occurred.
 */ 
UC_EXPORTED uc_err uc_cmd_line_app_run(uc_cmd_line_app *p, uc_cmd_line_opt *cmd_opt);

/*
 * Summary:
 *   Gets a static, default instance of the application.
 * Returns:
 *   The static, default instance of the application.
 */
UC_EXPORTED uc_cmd_line_app *uc_cmd_line_app_get_instance(void);

/*
 * Summary:
 *   Sets the function that the app uses to receive data.
 * Parameters:
 *   p: A pointer to the app whose property is set.
 *   value: A pointer to the function used to receive data.
 */
UC_EXPORTED void uc_cmd_line_app_set_receive(uc_cmd_line_app *p, uc_cmd_line_app_receive_func *value);

/*
 * Summary:
 *   Gets the function that the app uses to receive data.
 * Parameters:
 *   p: A pointer to the app whose property is returned.
 * Returns:
 *   A pointer to the function used to receive data.
 */ 
UC_EXPORTED uc_cmd_line_app_receive_func *uc_cmd_line_app_get_receive(uc_cmd_line_app *p);

/*
 * Summary:
 *   Gets the stateful object passed to the application's receive function.
 * Parameters:
 *   p: A pointer to the application object whose property is to be returned.
 * Returns:
 *   A pointer to the stateful object passed to the application's receive function.
 */
UC_EXPORTED void *uc_cmd_line_app_get_receive_state(uc_cmd_line_app *p);

/*
 * Summary:
 *   Sets the stateful object passed to the application's receive function.
 * Parameters:
 *   p: A pointer to the application object whose property is to be set.
 *   value: The stateful object passed to the application's receive function.
 */
UC_EXPORTED void uc_cmd_line_app_set_receive_state(uc_cmd_line_app *p, void *value);

/*
 * Summary:
 *   Sets the command used to quit the application.
 * Parameters:
 *   p: A pointer to the application.
 *   value: The value of the command that quits the application.
 */
UC_EXPORTED void uc_cmd_line_app_set_quit_command(uc_cmd_line_app *p, const char *value);

/*
 * Summary:
 *   Gets the value of the command that quits the application.
 * Parameters:
 *   p: A pointer to the application.
 * Returns:
 *   The value of the command that quits the application.
 */
UC_EXPORTED const char *uc_cmd_line_app_get_quit_command(uc_cmd_line_app *p);

/*
 * Summary:
 *   Sets the value of the command that shows help information.
 * Parameters:
 *   p: A pointer to the application whose property is set.
 *   value: The value of the command that shows help information.
 */
UC_EXPORTED void uc_cmd_line_app_set_help_command(uc_cmd_line_app *p, const char *value);

/*
 * Summary:
 *   Gets the value of the command that shows help information.
 * Parameters:
 *   p: A pointer to the app whose property is returned.
 * Returns:
 *   The value of the command that shows help information.
 */
UC_EXPORTED const char *uc_cmd_line_app_get_help_command(uc_cmd_line_app *p);

/*
 * Summary:
 *   Sets the command structure that the application uses.
 * Parameters:
 *   p: A pointer to the application.
 *   value: The command structure to be used by the application.
 */
UC_EXPORTED void uc_cmd_line_app_set_cmd(uc_cmd_line_app *p, uc_cmd_line *value);

/*
 * Summary:
 *   Gets the command structure used by the application.
 * Parameters:
 *   p: A pointer to the application.
 * Returns:
 *   A pointer to the command structure used by the application.
 */
UC_EXPORTED uc_cmd_line *uc_cmd_line_app_get_cmd(uc_cmd_line_app *p);

/*
 * Summary:
 *   Gets the command parser used by the application.
 * Parameters:
 *   p: A pointer to the application.
 * Returns:
 *   A pointer to the command parser used by the application.
 */
UC_EXPORTED uc_cmd_parser *uc_cmd_line_app_get_cmd_parser(uc_cmd_line_app *p);

#endif
