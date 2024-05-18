#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "common.h"

#define MAX_FILE_SUFFIX 4

typedef struct DecodeInfo
{
    /* Magic string */
    char *magic_string;
    int size_magic_string;

    /* Input .bmp file */
    char *steg_image_fname;
    FILE *fptr_steg_image;

    /* Output secret file */
    char *secret_fname_without_extn;
    char *secret_fname;
    FILE *fptr_secret_file;
    char secret_file_extn[MAX_FILE_SUFFIX];
    int size_secret_file;
    int secret_extn_size;
    

} DecodeInfo;



Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);


Status do_decoding(DecodeInfo *decInfo);

Status read_bmp_header(FILE *fptr_steg_image);

Status open_files_decode(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

unsigned char decode_byte_from_lsb(char *buffer);

Status decode_data_from_image(char *data, int size, FILE *fptr_steg_image);










#endif
