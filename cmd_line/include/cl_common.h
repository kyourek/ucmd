#ifndef CL_COMMON_H
#define CL_COMMON_H

/*
 * Definitions for command, argument, and
 * token sizes.
 */
#ifndef CL_CMD_LEN_MAX
#define CL_CMD_LEN_MAX                  200
#endif
#ifndef CL_TOK_LEN_MAX
#define CL_TOK_LEN_MAX                  CL_CMD_LEN_MAX
#endif
#ifndef CL_CMD_COUNT_MAX
#define CL_CMD_COUNT_MAX                20
#endif
#ifndef CL_ARG_COUNT_MAX
#define CL_ARG_COUNT_MAX                5
#endif
#ifndef CL_SWITCH_COUNT_MAX
#define CL_SWITCH_COUNT_MAX             10
#endif
#ifndef CL_RESPONSE_LEN_MAX
#define CL_RESPONSE_LEN_MAX             CL_CMD_LEN_MAX
#endif

/*
 * Defines snprintf when on Windows.
 */
#ifdef CL_WIN_API
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

/*
 * When compiling on Windows, CL_IMPORT can
 * be used when using this header file in
 * another program. CL_EXPORT should be defined 
 * when compiling this program itself.
 */
#ifdef CL_IMPORT
#define CL_EXPORTED __declspec(dllimport)
#else
#ifdef CL_EXPORT
#define CL_EXPORTED __declspec(dllexport)
#else
#define CL_EXPORTED
#endif
#endif

/*
 * Definitions for a boolean type.
 */
#define CL_BOOL     int
#define CL_FALSE    0
#define CL_TRUE     !CL_FALSE

/*
 * Definition for the type returned
 * by functions that use an error code.
 */
#define CL_ERR      int

#endif
