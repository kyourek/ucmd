#ifndef CL_CMD_LINE_VALIDATOR_H
#define CL_CMD_LINE_VALIDATOR_H

#include "cl_common.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_opt.h"

/*
 * Summary:
 *   Validator that validates command input against a command option.
 */
typedef struct cl_cmd_line_validator cl_cmd_line_validator;

/*
 * Summary:
 *   Validates the given command agains the given option.
 * Parameters:
 *   p: A pointer to the validator.
 *   cmd: The command to be validated.
 *   cmd_opt: The command option against which the command is validated.
 * Returns:
 *   Any error that was encountered during validation, or NULL if no errors
 *   exist.
 */
CL_EXPORTED const char *cl_cmd_line_validator_validate(cl_cmd_line_validator *p, cl_cmd_line *cmd, cl_cmd_line_opt *cmd_opt);

/*
 * Summary:
 *   Gets a static, default instance of the command validator.
 * Returns:
 *   The static, default instance of the command validator.
 */
CL_EXPORTED cl_cmd_line_validator *cl_cmd_line_validator_get_instance(void);

#endif
