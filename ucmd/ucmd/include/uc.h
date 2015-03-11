#ifndef UC_H
#define UC_H

/* Sets the size of the command buffer when using
   the command-line application framework. All
   entered commands must have a size equal to or
   less than this buffer's size. */
#ifndef ucCmdLineApp_CMD_BUF_LENGTH
#define ucCmdLineApp_CMD_BUF_LENGTH 200
#endif

/* Sets the size of the command response buffer.
   All response strings must have a size equal to
   or less than the size of this buffer to avoid
   truncation. */
#ifndef ucCmdLine_RESPONSE_LENGTH
#define ucCmdLine_RESPONSE_LENGTH 200
#endif

/* Sets the number of available command options.
   The number of created command options must be
   equal to or less than this number. */
#ifndef ucCmdLineOpt_COUNT
#define ucCmdLineOpt_COUNT 10
#endif

/* Sets the number of available switch options.
   The number of created switch options must be
   equal to or less than this number. */
#ifndef ucSwitchOpt_COUNT
#define ucSwitchOpt_COUNT 50
#endif

/* Sets the number of available argument options.
   This is the total number of options available
   to commands and switches, combined. */
#ifndef ucArgOpt_COUNT
#define ucArgOpt_COUNT 50
#endif

/* Sets the maximum expected length of a single
   token in a command line. */
#ifndef ucTok_LENGTH_MAX
#define ucTok_LENGTH_MAX ucCmdLineApp_CMD_BUF_LENGTH
#endif

/* Sets the numeric type of numeric argument
   options. */
#ifndef ucArgOpt_NUMERIC_TYPE
#define ucArgOpt_NUMERIC_TYPE double
#endif

/* Include this def when using the library with another
   program on Windows.
   Exported functions will be decorated with dllimport
   to make them available to external programs. */
#ifdef uc_DECLSPEC_DLLIMPORT
#define uc_EXPORTED uc_EXTERN_C __declspec(dllimport)
#endif

/* Include this def when compiling this program on
   Windows.
   Exported functions will be decorated with dllexport
   to make them available to external programs. */
#ifdef uc_DECLSPEC_DLLEXPORT
#define uc_EXPORTED uc_EXTERN_C __declspec(dllexport)
#endif

/* Prepend extern "C" if we're in a C++
   compiler. */
#ifdef __cplusplus
#define uc_EXTERN_C extern "C"
#else
#define uc_EXTERN_C
#endif

/* Default to setting uc_EXPORTED to the result
   of our extern "C" check. */
#ifndef uc_EXPORTED
#define uc_EXPORTED uc_EXTERN_C
#endif

#endif
