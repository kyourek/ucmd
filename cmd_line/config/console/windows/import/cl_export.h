#ifndef CL_EXPORT_H
#define CL_EXPORT_H

/* Include this file when using the library with another
   program. */

/* Exported functions will be decorated with dllimport
   to make them available to external programs. */
#define CL_EXPORTED CL_EXTERN_C __declspec(dllimport)

#endif
