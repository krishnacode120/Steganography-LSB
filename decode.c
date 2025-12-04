#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "common.h"
#include "types.h"

/* ------------------------------------------------------------------------ */
/* Helper: Skip BMP header (first 54 bytes) */
Status skip_bmp_header(FILE *fptr)
{
    fseek(fptr, 54, SEEK_SET);
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Helper: Extract a decoded byte from 8 LSBs */
Status decode_byte_from_lsb(char *buffer, char *decoded_byte)
{
    int i;
    *decoded_byte = 0;

    for (i = 0; i < 8; i++)
    {
        *decoded_byte <<= 1;
        *decoded_byte |= (buffer[i] & 1);
    }
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Step 1: Read & validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    decInfo->stego_image_fname = argv[2];

    /* If output filename is given */
    if (argv[3] != NULL)
        decInfo->output_fname = argv[3];
    else
        decInfo->output_fname = "decoded.txt";

    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Step 2: Open decoding files */
Status open_decode_files(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");

    /* Open stego image */
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (!decInfo->fptr_stego_image)
    {
        printf("ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    printf("INFO: Opened %s\n", decInfo->stego_image_fname);

    /* Open output file */
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    if (!decInfo->fptr_output)
    {
        printf("ERROR: Unable to open file %s\n", decInfo->output_fname);
        return e_failure;
    }
    printf("INFO: Opened %s\n", decInfo->output_fname);

    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Step 3: Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Magic String Signature\n");

    char buffer[8];
    char ch;

    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(buffer, &ch);

        if (ch != MAGIC_STRING[i])
        {
            printf("ERROR: Magic string mismatch. Image not stegged.\n");
            return e_failure;
        }
    }

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Step 4: Decode File Extension */
Status decode_secret_extn(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Output File Extension\n");

    char buffer[8];
    char ch;

    for (int i = 0; i < 4; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(buffer, &ch);
        decInfo->decoded_extn[i] = ch;
    }
    decInfo->decoded_extn[4] = '\0';

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Step 5: Decode File Size (32 bits) */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding File Size\n");

    char buffer[8];
    char bit;
    long size = 0;

    for (int i = 0; i < 32; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(buffer, &bit);

        size <<= 1;
        size |= (bit & 1);
    }

    decInfo->decoded_file_size = size;

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Step 6: Decode Secret File Data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding File Data\n");

    char buffer[8];
    char ch;

    for (long i = 0; i < decInfo->decoded_file_size; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(buffer, &ch);
        fputc(ch, decInfo->fptr_output);
    }

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* FINAL: Full Decoding Process */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: ## Decoding Procedure Started ##\n");

    /* 1. Open files */
    if (open_decode_files(decInfo) == e_failure)
        return e_failure;

    /* 2. Skip BMP header */
    skip_bmp_header(decInfo->fptr_stego_image);

    /* 3. Decode in sequence */
    if (decode_magic_string(decInfo) == e_failure)
        return e_failure;

    decode_secret_extn(decInfo);
    decode_secret_file_size(decInfo);
    decode_secret_file_data(decInfo);

    return e_success;
}
