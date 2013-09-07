#ifndef CL_COMMON_H
#define CL_COMMON_H

/*
 * Definitions for command, argument, and
 * token sizes.
 */
#ifndef CL_CMD_LEN_MAX
#define CL_CMD_LEN_MAX                  1000
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
 * Prepend extern "C" if we're in a C++
 * compiler
 */
#ifdef __cplusplus
#define CL_EXTERN_C extern "C"
#else
#define CL_EXTERN_C
#endif

/*
 * When compiling on Windows, CL_IMPORT can
 * be used when using this header file in
 * another program. CL_EXPORT should be defined 
 * when compiling this program itself.
 */
#ifdef CL_WIN_API
#ifdef CL_IMPORT
#define CL_EXPORTED CL_EXTERN_C __declspec(dllimport)
#else
#ifdef CL_EXPORT
#define CL_EXPORTED CL_EXTERN_C __declspec(dllexport)
#endif
#endif
#endif

/*
 * Default to setting CL_EXPORTED to the result
 * of our extern "C" check.
 */
#ifndef CL_EXPORTED
#define CL_EXPORTED CL_EXTERN_C
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
