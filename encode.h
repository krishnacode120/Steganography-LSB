#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>
#include "types.h"  // Contains user-defined types (uint, Status, OperationType)

/* ------------------------------------------------------------------------
 * Structure to store all information required for encoding
 * ------------------------------------------------------------------------ */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4     // extn: e.g., .txt -> 4 chars

typedef struct _EncodeInfo
{
    /* Source Image Info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;

/* ------------------------------------------------------------------------
 * Function Prototypes Required for Encoding
 * ------------------------------------------------------------------------ */

/* Check operation type */
OperationType check_operation_type(char *arg);

/* Read & validate command line args */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform full encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Open all files needed for encoding */
Status open_files(EncodeInfo *encInfo);

/* Check capacity of source image */
Status check_capacity(EncodeInfo *encInfo);

/* Get BMP image size (width * height * 3) */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size (secret.txt size) */
uint get_file_size(FILE *fptr);

/* Copy header (first 54 bytes) */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Encode magic string */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode extension (e.g., "txt") */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode size of secret file (32 bits) */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data (character by character) */
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode 1 byte → 8 image bytes (LSB) */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into array of 8 bytes */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
