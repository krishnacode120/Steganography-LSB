#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

int main()
{
    EncodeInfo encInfo;

    /* Assign sample input/output files */
    encInfo.src_image_fname   = "beautiful.bmp";
    encInfo.secret_fname      = "secret.txt";
    encInfo.stego_image_fname = "stego_img.bmp";

    printf("----- TESTING ENCODE FUNCTIONS -----\n\n");

    /* Test Argument Validation */
    char *argv_test[] = {"./a.out", "-e", "beautiful.bmp", "secret.txt", "stego_img.bmp"};
    if (read_and_validate_encode_args(argv_test, &encInfo) == e_success)
        printf("SUCCESS: read_and_validate_encode_args() working\n");
    else
        printf("ERROR: read_and_validate_encode_args() failed\n");

    /* Test File Opening */
    if (open_files(&encInfo) == e_success)
        printf("SUCCESS: open_files() working\n");
    else
    {
        printf("ERROR: open_files() failed\n");
        return 1;
    }

    /* Test Image Size */
    uint img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u bytes\n", img_size);

    /* Test Secret File Size */
    encInfo.size_secret_file = get_file_size(encInfo.fptr_secret);
    printf("INFO: Secret file size = %ld bytes\n", encInfo.size_secret_file);

    /* Test Capacity Check */
    if (check_capacity(&encInfo) == e_success)
        printf("SUCCESS: check_capacity() working\n");
    else
        printf("ERROR: check_capacity() failed\n");

    /* Test Header Copy */
    FILE *temp_out = fopen("temp_header_test.bmp", "w");
    if (temp_out)
    {
        copy_bmp_header(encInfo.fptr_src_image, temp_out);
        printf("SUCCESS: copy_bmp_header() working\n");
        fclose(temp_out);
    }

    /* Reset file pointer after header copy for next tests */
    fseek(encInfo.fptr_src_image, 54, SEEK_SET);

    /* Test Magic String Encoding */
    if (encode_magic_string(MAGIC_STRING, &encInfo) == e_success)
        printf("SUCCESS: encode_magic_string() working\n");

    printf("\n----- BASIC TESTS COMPLETED -----\n");

    return 0;
}
