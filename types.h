#ifndef TYPES_H
#define TYPES_H

/* ------------------------------------------------------------------------
 * Custom Data Types Used Across the Project
 * ------------------------------------------------------------------------ */

/* Unsigned integer (used for image sizes, file sizes) */
typedef unsigned int uint;

/* Generic status used as function return type */
typedef enum
{
    e_success,
    e_failure
} Status;

/* Operation modes for main program */
typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
