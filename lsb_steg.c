/************************************************************************************
 *  Project     : LSB Image Steganography
 *  File Name   : lsb_steg.c
 *  Description : This is the main controller file for the Steganography project.
 *                It parses command-line arguments, identifies whether the user
 *                intends to perform encoding (-e) or decoding (-d), validates the
 *                input parameters, and calls the respective functional modules.
 *
 *  Operations:
 *      Encoding : ./lsb_steg -e <input.bmp> <secret.txt> [output_stego.bmp]
 *      Decoding : ./lsb_steg -d <stego.bmp> [decoded_output.txt]
 *
 *  Author      : Krishna Moorthy T
 *  Date        : 2025-12-04
 ************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

/* Print Usage */
void print_usage()
{
    printf("Usage:\n");
    printf("Encoding: ./lsb_steg -e <.bmp file> <secret file> [output bmp]\n");
    printf("Decoding: ./lsb_steg -d <stego.bmp> [output file]\n");
}

/* Check operation type */
OperationType check_operation_type(char *arg)
{
    if (strcmp(arg, "-e") == 0)
        return e_encode;
    else if (strcmp(arg, "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        print_usage();
        return 1;
    }

    OperationType opt = check_operation_type(argv[1]);

    /* ---------------------- ENCODING ---------------------- */
    if (opt == e_encode)
    {
        EncodeInfo encInfo;

        /* Minimum args = 4 → ./lsb_steg -e img.bmp secret.txt */
        if (argc < 4)
        {
            print_usage();
            return 1;
        }

        if (read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            print_usage();
            return 1;
        }

        if (do_encoding(&encInfo) == e_success)
            printf("INFO: ## Encoding Done Successfully ##\n");
        else
            printf("ERROR: Encoding Failed\n");
    }

    /* ---------------------- DECODING ---------------------- */
    else if (opt == e_decode)
    {
        DecodeInfo decInfo;

        /* Minimum args = 3 → ./lsb_steg -d stego.bmp */
        if (argc < 3)
        {
            print_usage();
            return 1;
        }

        if (read_and_validate_decode_args(argv, &decInfo) == e_failure)
        {
            print_usage();
            return 1;
        }

        if (do_decoding(&decInfo) == e_success)
            printf("INFO: ## Decoding Done Successfully ##\n");
        else
            printf("ERROR: Decoding Failed\n");
    }

    /* ---------------------- UNSUPPORTED ---------------------- */
    else
    {
        print_usage();
        return 1;
    }

    return 0;
}
