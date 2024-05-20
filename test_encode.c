/*
* Name        : Koushik K
* Date        : 26-02-2024
* Description : Project - LSB Steganography (Encode and Decode)
* Sample I/P  : ./lsb_steg -e beautiful.bmp secret.txt output_stego.bmp
* Sample O/P  : Encoding successfully completed
*/

#include <stdio.h>
#include "decode.h"
#include "encode.h"
#include "types.h"

int main(int argc, char **argv)
{
    
    if (argc >= 3)
    {

	int ret = check_operation_type(argv);

	
	if (ret == e_encode)
	{
	    EncodeInfo encInfo;
	    //Selected encoding	    
	    //puts("INFO: Selected encoding");
	    if (read_and_validate_encode_args(argv, &encInfo) == e_success)
	    {
		char arr[100];
		printf("Enter magic string : ");
		scanf("%[^\n]", arr);
		getchar();
		encInfo.magic_string = malloc(strlen(arr) + 1);
		strncpy(encInfo.magic_string, arr, strlen(arr));
		encInfo.magic_string[strlen(arr)] = '\0';
		
		// success message for read_and_validate_encode_args
		if (do_encoding(&encInfo) == e_success)
		{
		    puts("INFO: Encoding successfully completed##");
		    return e_success;
		}
		else
		{
		    puts("INFO: Encoding failed");
		    return e_failure;
		}

	    }
	    else
	    {
		puts("INFO: Failure in parsing input arguments");
		puts("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output .bmp file]");
		return e_failure;
	    }

	}
	else if (ret == e_decode)
	{
	    DecodeInfo decInfo;
	    //Selected decoding
	    //puts("INFO: Selected decoding");
	    if (read_and_validate_decode_args(argv, &decInfo) == e_success)
	    {
		char arr[100];
		printf("Enter magic string : ");
		scanf("%[^\n]", arr);
		getchar();
		decInfo.magic_string = malloc(strlen(arr) + 1);
		strncpy(decInfo.magic_string, arr, strlen(arr));
		decInfo.magic_string[strlen(arr)] = '\0';

		if (do_decoding(&decInfo) == e_success)
		{
		    puts("INFO: Decoding succesfully completed##");
		    return e_success;
		}
		else
		{
		    puts("INFO: Decoding Failed");
		    return e_failure;
		}
	    }
	    else
	    {
		puts("INFO: Failure in parsing input arguments");
		puts("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]");
		return e_failure;
	    }
	}
	else
	{
	    //Error: Message
	    puts("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output .bmp file]");
	    puts("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]");
	    return e_failure;
	}

    }
    else
    {
	//Error : Message
	puts("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output .bmp file]");
	puts("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]");
	return e_failure;
    }


    return e_success;
}

OperationType check_operation_type(char *argv[])
{
    //if argv[1] is -e 
    	//return e_encode
    //else if argv[1] is -d 
   	//return e_decode
    //else
    	//return e_unsupported

    if (strcmp(argv[1], "-e") == 0)
    {
	return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
	return e_decode;
    }
    else
    {
	return e_unsupported;
    }
}
