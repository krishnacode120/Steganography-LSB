#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/* ------------------------------------------------------------------------ */
/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);

    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    return width * height * 3;
}

/* ------------------------------------------------------------------------ */
/* Open all files for encoding */
Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (!encInfo->fptr_src_image)
    {
        perror("fopen");
        printf("ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (!encInfo->fptr_secret)
    {
        perror("fopen");
        printf("ERROR: Unable to open file %s\n", encInfo->secret_fname);
        return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (!encInfo->fptr_stego_image)
    {
        perror("fopen");
        printf("ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Read & validate arguments */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    encInfo->src_image_fname   = argv[2];
    encInfo->secret_fname      = argv[3];

    /* If output filename not given */
    if (argv[4] != NULL)
        encInfo->stego_image_fname = argv[4];
    else
        encInfo->stego_image_fname = "stegged_img.bmp";

    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Check if secret file will fit in image */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    /* Required bits = magic + ext + size + actual data */
    uint required_size = 
          strlen(MAGIC_STRING) * 8
        + MAX_FILE_SUFFIX * 8 
        + 32
        + encInfo->size_secret_file * 8;

    if (encInfo->image_capacity < required_size)
    {
        printf("ERROR: Image does not have capacity to store secret file.\n");
        return e_failure;
    }

    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Copy BMP header to stego file */
Status copy_bmp_header(FILE *src, FILE *dest)
{
    rewind(src);

    char header[54];
    fread(header, 1, 54, src);
    fwrite(header, 1, 54, dest);

    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Encode 1 byte into 8 bytes (LSB) */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        char bit = (data >> (7 - i)) & 1;
        image_buffer[i] = (image_buffer[i] & ~1) | bit;
    }
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Read 8 bytes from source and encode one byte */
Status encode_data_to_image(char *data, int size, FILE *src, FILE *dest)
{
    for (int i = 0; i < size; i++)
    {
        fread((char *)data, 1, 8, src);       // read 8 image bytes
        encode_byte_to_lsb(data[i], data);    // modify LSBs
        fwrite(data, 1, 8, dest);             // write modified bytes
    }
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Encode Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("INFO: Encoding Magic String Signature\n");

   size_t len = strlen(magic_string);
   for (size_t i = 0; i < len; i++)
   {
        fread(encInfo->image_data, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i], encInfo->image_data);
        fwrite(encInfo->image_data, 1, 8, encInfo->fptr_stego_image);
    }

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Encode secret file extension (Always 4 characters) */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Extension\n");

    for (int i = 0; i < MAX_FILE_SUFFIX; i++)
    {
        fread(encInfo->image_data, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i], encInfo->image_data);
        fwrite(encInfo->image_data, 1, 8, encInfo->fptr_stego_image);
    }

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Encode secret file size (32 bits) */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Size\n");

    for (int i = 31; i >= 0; i--)
    {
        char bit = (file_size >> i) & 1;
        fread(encInfo->image_data, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(bit, encInfo->image_data);
        fwrite(encInfo->image_data, 1, 8, encInfo->fptr_stego_image);
    }

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Encode secret file data (byte-by-byte) */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Data\n");

    rewind(encInfo->fptr_secret);

    char ch;
    while (fread(&ch, 1, 1, encInfo->fptr_secret))
    {
        fread(encInfo->image_data, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(ch, encInfo->image_data);
        fwrite(encInfo->image_data, 1, 8, encInfo->fptr_stego_image);
    }

    printf("INFO: Done\n");
    return e_success;
}

/* ------------------------------------------------------------------------ */
/* Copy remaining data from src image to stego image */
Status copy_remaining_img_data(FILE *src, FILE *dest)
{
    char buffer[1];
    while (fread(buffer, 1, 1, src))
        fwrite(buffer, 1, 1, dest);

    return e_success;
}

/* ------------------------------------------------------------------------ */
/* MAIN ENCODING PIPELINE */
Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO: ## Encoding Procedure Started ##\n");

    /* Step 1: Open files */
    printf("INFO: Opening required files\n");
    if (open_files(encInfo) == e_failure)
        return e_failure;
    printf("INFO: Opened %s\n", encInfo->src_image_fname);
    printf("INFO: Opened %s\n", encInfo->secret_fname);
    printf("INFO: Opened %s\n", encInfo->stego_image_fname);
    printf("INFO: Done\n");

    /* Step 2: Check secret file size */
    printf("INFO: Checking for %s size\n", encInfo->secret_fname);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if (encInfo->size_secret_file == 0)
    {
        printf("ERROR: Secret file is empty\n");
        return e_failure;
    }
    printf("INFO: Done. Not Empty\n");

    /* Step 3: Check capacity */
    printf("INFO: Checking for %s capacity to handle %s\n",
           encInfo->src_image_fname, encInfo->secret_fname);

    if (check_capacity(encInfo) == e_failure)
        return e_failure;

    printf("INFO: Done. Found OK\n");

    /* Step 4: Copy header */
    printf("INFO: Copying Image Header\n");
    copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("INFO: Done\n");

    /* Step 5: Encode Magic String */
    encode_magic_string(MAGIC_STRING, encInfo);

    /* Step 6: Extract extension */
    strcpy(encInfo->extn_secret_file, ".txt");  // For now fixed

    /* Step 7: Encode file extension */
    encode_secret_file_extn(encInfo->extn_secret_file, encInfo);

    /* Step 8: Encode secret file size */
    encode_secret_file_size(encInfo->size_secret_file, encInfo);

    /* Step 9: Encode secret file content */
    encode_secret_file_data(encInfo);

    /* Step 10: Copy remaining bytes */
    printf("INFO: Copying Left Over Data\n");
    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("INFO: Done\n");

    return e_success;
}
