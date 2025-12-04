#ifndef COMMON_H
#define COMMON_H

#include "types.h"

/* Magic string to identify whether stegged or not */
#define MAGIC_STRING "#*"

/* ------------------------------------------------------------------------
 * Common utility function prototypes
 * These are shared by encoding and decoding modules
 * ------------------------------------------------------------------------ */

/* Get size of a file */
uint get_file_size(FILE *fptr);



#endif
