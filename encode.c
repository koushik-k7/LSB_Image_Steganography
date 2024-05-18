#include <stdio.h>
#include "encode.h"
#include "types.h"


/* Function Definitions */
/* Read and validate encode arguments
 * Input: argv, src and dest image ptr
 * Output: e_success or e_failure
 * Description: validate the input arguments
 */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *temp_str;

    if ((temp_str = strrchr(argv[2], '.')) != NULL && strcmp(temp_str, ".bmp") == 0)
    {
	encInfo->src_image_fname = argv[2];
    }
    else
    {
	return e_failure;
    }


    if ((temp_str = strrchr(argv[3], '.')) != NULL)
    {
	    encInfo->secret_fname = argv[3];
	    strcpy(encInfo->extn_secret_file, temp_str);
    }
    else
    {
	return e_failure;
    }


    //check argv[4] is passed or not
    //yes -> valid .bmp and store in encInfo->stego_image_fname = argv[4]
    //no -->
    //store encInfo->stego_image_fname = "stego.bmp"

    if (argv[4] != NULL) 
    {
	if((temp_str = strrchr(argv[4], '.')) != NULL && strcmp(temp_str, ".bmp") == 0)
	{
	    encInfo->stego_image_fname = argv[4];
	}
	else
	{
	    return e_failure;
	    //encInfo->stego_image_fname = "stego.bmp";
	}
    }
    else
    {
	encInfo->stego_image_fname = "stego.bmp";
    }

    return e_success;

}


/* Do the encoding
 * Input: src and dest image ptr
 * Output: e_success or e_failure
 * Description: encodes the data
 */
Status do_encoding(EncodeInfo *encInfo)
{
    if ( open_files(encInfo) == e_success)
    {
	puts("INFO: Files opened successfully");
	if(check_capacity(encInfo) == e_success)
	{
	    puts("INFO: Enough storage to store data");
	    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
	    {
		puts("INFO: Header copied successfully");
		if (encode_magic_string( encInfo->magic_string , encInfo) == e_success)
		{
		    puts("INFO: Magic string encoded successfully");
		    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
		    {
			puts("INFO: Secret file extension size encoded successfully");
			if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
			{
			    puts("INFO: Secret file extension encoded successfully");
			    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
			    {
				puts("INFO: Secret file size encoded successfully");
				if (encode_secret_file_data(encInfo) == e_success)
				{
				    puts("INFO: Secret file data encoded successfully");
				    if ( copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
				    {
					puts("INFO: Remaining data copied successfully");
					return e_success;
				    }
				    else
				    {
					puts("INFO: Failure in copying remaining data");
					return e_failure;
				    }
				}
				else
				{
				    puts("INFO: Failure in encoding secret file data");
				    return e_failure;
				}
			    }
			    else
			    {
				puts("INFO: Failure in encoding secret file size");
				return e_failure;
			    }
			}
			else
			{
			    puts("INFO: Failure in encoding secret file extension");
			    return e_failure;
			}
		    }
		    else
		    {
			puts("INFO: Failure in encoding secret file extension size");
			return e_failure;
		    }
		}
		else
		{
		    puts("INFO: Failure in encoding magic string");
		    return e_failure;
		}
	    }
	    else
	    {
		puts("INFO: Failure in copying bmp header");
		return e_failure;
	    }
	}
	else
	{
	    puts("INFO: Failure in checking capacity");
	    return e_failure;
	}
    }
    else
    {
	puts("INFO: Failure in opening files");
	return e_failure;
    }
}

/* Encode secret file extension 
 * Input: file extension, src and dest image ptr
 * Output: e_success or e_failure
 * Description: encodes the secret file extension
 */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    if (encode_data_to_image(file_extn,strlen(encInfo->extn_secret_file), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

/* Encode secret file size
 * Input: file size, src and dest image ptr
 * Output: e_success or e_failure
 * Description: encodes the secret file size
 */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    int size = file_size;
    char *ptr = (char *)&size;
    if (encode_data_to_image(ptr, sizeof(int), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

/* Encode secret file data
 * Input: src and dest image ptr
 * Output: e_success or e_failure
 * Description: encodes the secret file data
 */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[encInfo->size_secret_file];
    fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret);

    if (encode_data_to_image(buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

/* Copy remaining image data
 * Input: src and dest image ptr
 * Output: e_success or e_failure
 * Description: copies data from src img to dest img
 */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while (!feof(fptr_src))
    {
	ch = getc(fptr_src);
	putc(ch, fptr_dest);
    }
    if (feof(fptr_src))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

/* Encode secret file extension size
 * Input: file size, src and dest image ptr
 * Output: e_success or e_failure
 * Description: encodes the secret file extension size
 */
Status encode_secret_file_extn_size(long file_size, EncodeInfo *encInfo)
{
    
    int size = file_size;
    char *ptr = (char *)&size;
    if (encode_data_to_image(ptr, sizeof(int), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
     
}

/* Encode magic string
 * Input: magic string, src and dest file ptr
 * Output: e_success or e_failure
 * Description: encodes the magic string 
 */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encInfo->size_magic_string = strlen(magic_string);
    int size = encInfo->size_magic_string;
    char *ptr = (char *) &size;

    if (encode_data_to_image(ptr, sizeof(int), encInfo->fptr_src_image, encInfo->fptr_stego_image)  == e_success)
    {

	if (encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
	{
	    return e_success;
	}
	else
	{
	    return e_failure;
	}
    }
    else
    {
	return e_failure;
    }

}

/* Encode data to image
 * Input: char arr, total size, src and dest file ptr
 * Output: e_success or e_failure
 * Description: For each byte in char arr reads 8 bytes from src, encode, and store in dest
 */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
	fread(buffer, 8, 1, fptr_src_image);
	encode_byte_to_lsb(data[i], buffer);
	fwrite(buffer, 8, 1, fptr_stego_image);
    }
    return e_success;

}

/* Encode byte to lsb
 * Input: char data, buffer of 8 chars
 * Output: e_success or e_failure
 * Description: encodes data into 8 bytes of data in the buffer
 */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 7; i >= 0; i--)
    {
	if (data & 1 << i)
	{
	    image_buffer[7-i] |= 1;
	}
	else
	{
	    image_buffer[7-i] &= ~1;
	}
    }
    return e_success;
}

/* Copy bmp header
 * Input: src image file ptr, dest image file ptr
 * Output: e_success or e_failure
 * Description: Copy the .bmp header from source file to dest file
 */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    fseek(fptr_src_image, 0, SEEK_SET);
    char buffer[54];
    if(fread(buffer, 54, 1, fptr_src_image) == 1)
    {
	fwrite(buffer, 54, 1, fptr_dest_image);
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

/* Check capacity of src image
 * Input: src image file ptr, secret file ptr
 * Output: e_success or e_failure
 * Description: Checks src img size to store secret file's data
 */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    int secret_code_size = 4 + (strlen(encInfo->magic_string) + 4 + 4 + strlen(encInfo->extn_secret_file) + encInfo->size_secret_file) * 8;

    if ( encInfo->image_capacity > secret_code_size)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

/* Get file size
 * Input: File ptr
 * Output: size of the file
 * Description: Returns size of the file
 */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

/* Get image size
 * Input: Image file ptr 
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
