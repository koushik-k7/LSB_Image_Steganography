#include "types.h"
#include "decode.h"

/* Do the decoding
 * Input: input and output files
 * Output: e_success or e_failure
 * Description: decode the input image and stores the data into output file
 */
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_files_decode(decInfo) == e_success)
    {
	puts("INFO: Files opened successfully");
	if (read_bmp_header(decInfo->fptr_steg_image) == e_success)
	{
	    puts("INFO: Bmp header read successfully");
	    if (decode_magic_string(decInfo) == e_success)
	    {
		puts("INFO: Magic string decoded successfully");
		if (decode_secret_file_extn_size(decInfo) == e_success)
		{
		    puts("INFO: File extension size decoded successfully");
		    if (decode_secret_file_extn(decInfo) == e_success)
		    {
			puts("INFO: File extension decoded successfully");
			if (decode_secret_file_size(decInfo) == e_success)
			{
			    puts("INFO: File size decoded successfully");
			    if (decode_secret_file_data(decInfo) == e_success)
			    {
				puts("INFO: File data decoded successfully");
				return e_success;
			    }
			    else
			    {
				puts("INFO: Failure in decoding data");
				return e_failure;
			    }
			}
			else
			{
			    puts("INFO: Failure in decoding file size");
			    return e_failure;
			}
		    }
		    else
		    {
			puts("INFO: Failure in decoding file extension");
			return e_failure;
		    }
		}
		else
		{
		    puts("INFO: Failure in decoding file extension size");
		    return e_failure;
		}
	    }
	    else
	    {
		puts("INFO: Failure in decoding magic string");
		return e_failure;
	    }
	}
	else
	{
	    puts("INFO: Failure in reading header");
	    return e_failure;
	}
    }
    else
    {
	puts("INFO: Failure in opening files");
	return e_failure;
    }

}




/* Read the bmp file header
 * Input: input file
 * Output: e_success or e_failure
 * Description: skips .bmp file header and moves to rgb data
 */
Status read_bmp_header(FILE *fptr_steg_image)
{
    fseek(fptr_steg_image, 54, SEEK_SET);
    return e_success;

}

/* Read and validate the input arguments
 * Input: cla
 * Output: e_success or e_failure
 * Description: read and validate the input arguments
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *temp_ptr;
    if ((temp_ptr = strrchr(argv[2], '.')) != NULL  && strcmp(temp_ptr, ".bmp") == 0)
    {
	decInfo->steg_image_fname = argv[2];
    }
    else
    {
	return e_failure;
    }
    
    if ( argv[3] != NULL )
    {
	decInfo->secret_fname_without_extn = argv[3];
	//strcpy(decInfo->secret_file_extn, temp_ptr);
    }
    else
    {
	decInfo->secret_fname_without_extn = "secret_msg";
	//strcpy(decInfo->secret_file_extn, ".txt");

    }
    return e_success;
}


/* Opens the input and output file
 * Input: input and output files
 * Output: e_success or e_failure
 * Description: opens the input and output file to decode
 */
Status open_files_decode(DecodeInfo *decInfo)
{
    if ( (decInfo->fptr_steg_image = fopen(decInfo->steg_image_fname, "r")) == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->steg_image_fname);
	//error
	return e_failure;
    }

    /*
    if ( (decInfo->fptr_secret_file = fopen(decInfo->secret_fname, "w")) == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
	//error
	return e_failure;
    }
    */
    return e_success;
}


/* Decodes the magic string
 * Input: input file
 * Output: e_success or e_failure
 * Description: decode the input image and checks for magic string
 */
Status decode_magic_string(DecodeInfo *decInfo)
{
    /* decode magic_string size */

    union
    {
	unsigned int data_size;
	char size_buffer[5];

    } size;

    size.size_buffer[5] = 0;

    decode_data_from_image(size.size_buffer, 4, decInfo->fptr_steg_image);

    decInfo->size_magic_string = size.data_size;


    if (decInfo->size_magic_string != strlen(decInfo->magic_string))
    {
	return e_failure;
    }

    /* decode magic string */
    char magic_string[strlen(decInfo->magic_string) + 1];
    magic_string[strlen(decInfo->magic_string)] = 0;

    decode_data_from_image(magic_string, strlen(decInfo->magic_string), decInfo->fptr_steg_image);

    if (strcmp(magic_string, decInfo->magic_string) == 0)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

/* Decodes the secret file extension size
 * Input: input file
 * Output: e_success or e_failure
 * Description: decode the input image and checks for secret file extension size
 */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    union
    {
	unsigned int data_size;
	char size_buffer[5];

    } size;

    size.size_buffer[5] = 0;

    decode_data_from_image(size.size_buffer, 4, decInfo->fptr_steg_image);

    decInfo->secret_extn_size = size.data_size;

    return e_success;

}

/* Decodes the secret file size
 * Input: input file
 * Output: e_success or e_failure
 * Description: decode the input image and checks for secret file size
 */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    union
    {
	unsigned int data_size;
	char size_buffer[5];

    } size;

    size.size_buffer[5] = 0;

    decode_data_from_image(size.size_buffer, 4, decInfo->fptr_steg_image);

    decInfo->size_secret_file = size.data_size;

    return e_success;
}

/* Decodes the secret file extn
 * Input: input file
 * Output: e_success or e_failure
 * Description: decode the input image and checks for secret file extension
 */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extn_size[decInfo->secret_extn_size];

    decode_data_from_image(extn_size, decInfo->secret_extn_size, decInfo->fptr_steg_image);



    decInfo->secret_fname = malloc( strlen(decInfo->secret_fname_without_extn) + sizeof(extn_size) + 1);

    strcpy(decInfo->secret_fname, decInfo->secret_fname_without_extn);
    strcat(decInfo->secret_fname, extn_size);

    if ((decInfo->fptr_secret_file = fopen(decInfo->secret_fname, "w")) == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->steg_image_fname);
	//error
	return e_failure;
    }

    /*
    if (strcmp(extn_size, decInfo->secret_file_extn) != 0)
    {
	char *ptr = strrchr(decInfo->secret_fname, '.');
	strcpy(ptr, extn_size);
    }
    */
    return e_success;

}

/* Decodes the secret data
 * Input: input file and output file
 * Output: e_success or e_failure
 * Description: decode the input image and stores data in output image
 */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char data_arr[decInfo->size_secret_file];

    decode_data_from_image(data_arr, decInfo->size_secret_file, decInfo->fptr_steg_image);

    if (fwrite(data_arr, decInfo->size_secret_file, 1, decInfo->fptr_secret_file) == 1)
    {
	return e_success;
    }
    return e_failure;

}

/* Decodes the data from img
 * Input: input file, data
 * Output: e_success or e_failure
 * Description: decode the input image
 */
Status decode_data_from_image(char *data, int size, FILE *fptr_steg_image)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
	fread(buffer, 8, 1, fptr_steg_image);
	data[i] = decode_byte_from_lsb(buffer);
    }
    return e_success;
}

/* Decodes the bytes from lsb
 * Input: input img data
 * Output: character
 * Description: decode the input image and returns the character
 */
unsigned char decode_byte_from_lsb(char *buffer)
{
    unsigned char ch = 0;

    for (int i = 0; i < 8; i++)
    {
	if (buffer[i] & 1)
	{
	    ch = (ch << 1) | 1;
	}
	else
	{
	    ch = ch << 1;
	}
    }

    return ch;
}


