#ifndef UC_EXPORT_H
#define UC_EXPORT_H

/* Include this file when using the library with another
   program. */

/* Exported functions will be decorated with dllimport
   to make them available to external programs. */
#define UC_EXPORTED UC_EXTERN_C __declspec(dllimport)

#endif
