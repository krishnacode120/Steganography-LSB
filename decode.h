#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

/* 
 * Structure to store information required for
 * decoding data from stego Image
 */

typedef struct _DecodeInfo
{
    /* Stego image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output file info */
    char *output_fname;
    FILE *fptr_output;

    /* Decoded data */
    char decoded_extn[5];       // Stores decoded extension (ex: txt)
    long decoded_file_size;     // Original secret file size

} DecodeInfo;

/* ---------------- DECODING FUNCTION PROTOTYPES ---------------- */

/* Read and validate arguments for decoding */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open all necessary decode files */
Status open_decode_files(DecodeInfo *decInfo);

/* Skip BMP header (54 bytes) */
Status skip_bmp_header(FILE *fptr);

/* Decode magic string validation */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode file extension */
Status decode_secret_extn(DecodeInfo *decInfo);

/* Decode file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode actual secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode 1 byte from 8 LSB bits */
Status decode_byte_from_lsb(char *buffer, char *decoded_byte);

#endif
