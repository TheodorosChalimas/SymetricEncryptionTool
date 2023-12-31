#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>

#define BLOCK_SIZE 16


/* function prototypes */
void print_hex(unsigned char *, size_t);
void print_string(unsigned char *, size_t);
void usage(void);
void check_args(char *, char *, unsigned char *, int, int);
void keygen(unsigned char *, unsigned char *,unsigned int);
void encrypt(unsigned char *, int, unsigned char *, unsigned char *, int );
int decrypt(unsigned char *, int, unsigned char *, unsigned char *, int);
void gen_cmac(unsigned char *, size_t, unsigned char *, unsigned char *, int);
int verify_cmac(unsigned char *, unsigned char *);



/* TODO Declare your function prototypes here... */



/*
 * Prints the hex value of the input
 * 16 values per line
 */
void
print_hex(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++) {
			if (!(i % 16) && (i != 0))
				printf("\n");
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
}


/*
 * Prints the input as string
 */
void
print_string(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++)
			printf("%c", data[i]);
		printf("\n");
	}
}


/*
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void
usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "    assign_1 -i in_file -o out_file -p passwd -b bits"
	        " [-d | -e | -s | -v]\n"
	    "    assign_1 -h\n"
	);
	printf(
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -p    psswd   Password for key generation\n"
	    " -b    bits    Bit mode (128 or 256 only)\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -s            Encrypt+sign input and store results to output\n"
	    " -v            Decrypt+verify input and store results to output\n"
	    " -h            This help message\n"
	);
	exit(EXIT_FAILURE);
}


/*
 * Checks the validity of the arguments
 * Check the new arguments you introduce
 */
void
check_args(char *input_file, char *output_file, unsigned char *password,
    int bit_mode, int op_mode)
{
	if (!input_file) {
		printf("Error: No input file!\n");
		usage();
	}

	if (!output_file) {
		printf("Error: No output file!\n");
		usage();
	}

	if (!password) {
		printf("Error: No user key!\n");
		usage();
	}

	if ((bit_mode != 128) && (bit_mode != 256)) {
		printf("Error: Bit Mode <%d> is invalid!\n", bit_mode);
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


/*
 * Generates a key using a password
 */
void
keygen(unsigned char *password, unsigned char *key,unsigned int bit_mode)
{
	/* TODO Task A */
  EVP_Digest (password, strlen((char*)password), key, NULL, EVP_sha1(), NULL);


  return;
}

/*
 * Encrypts the data
 */
void
encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext, int bit_mode)
{
  int len;
	/* TODO Task B */
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (bit_mode==256) {
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL);
  } else if (bit_mode==128) {
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL);
  } else {
    usage();
    return;
  }
  EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);

  EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);

  EVP_CIPHER_CTX_free(ctx);
  return;
}


/*
 * Decrypts the data and returns the plaintext size
 */
int
decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext, int bit_mode)
{
  /* TODO Task C */
  int len=0;
	int plaintext_len=0;
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (bit_mode==256) {
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL);
  } else if (bit_mode==128) {
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL);
  } else {
    usage();
    return -1;
  }
  EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
  plaintext_len = len;
  EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
	plaintext_len += len;

  EVP_CIPHER_CTX_free(ctx);
  return plaintext_len;
}


/*
 * Generates a CMAC
 */
void
gen_cmac(unsigned char *data, size_t data_len, unsigned char *key,
    unsigned char *cmac, int bit_mode)
{
  size_t len;
  CMAC_CTX *ctx = CMAC_CTX_new();
  if (bit_mode==256) {
      CMAC_Init(ctx, key, 32, EVP_aes_256_cbc(), NULL);
  } else if (bit_mode==128) {
      CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);
  } else {
    usage();
    return;
  }
  CMAC_Update(ctx, data, data_len);
  CMAC_Final(ctx, cmac, &len);
  CMAC_CTX_free(ctx);
  return;

}


/*
 * Verifies a CMAC
 */
int
verify_cmac(unsigned char *cmac1, unsigned char *cmac2)
{
  if(strcmp((char*)cmac1,(char*)cmac2)==0){
    printf("verified\n" );
    return 1;
  }else{
    printf("failed to verify\n" );
    return 0;
  }
}



/* TODO Develop your functions here... */



/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int
main(int argc, char **argv)
{
	int opt;			/* used for command line arguments */
	unsigned int bit_mode;			/* defines the key-size 128 or 256 */
	int op_mode;			/* operation mode */
	char *input_file;		/* path to the input file */
	char *output_file;		/* path to the output file */
	unsigned char *password;	/* the user defined password */

	/* Init arguments */
	input_file = NULL;
	output_file = NULL;
	password = NULL;
	bit_mode = -1;
	op_mode = -1;


	/*
	 * Get arguments
	 */
	while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:")) != -1) {
		switch (opt) {
		case 'b':
			bit_mode = atoi(optarg);
			break;
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'p':
			password = (unsigned char *)strdup(optarg);
			break;
    case 'e':
      /* if op_mode == 0 the tool encrypts */
      op_mode = 0;
      break;
		case 'd':
			/* if op_mode == 1 the tool decrypts */
			op_mode = 1;
			break;
		case 's':
			/* if op_mode == 2 the tool signs */
			op_mode = 2;
			break;
		case 'v':
			/* if op_mode == 3 the tool verifies */
			op_mode = 3;
			break;
		case 'h':
		default:
			usage();
		}
	}

	/* check arguments */
	check_args(input_file, output_file, password, bit_mode, op_mode);


	/* TODO Develop the logic of your tool here... */

	/* Initialize the library */
  unsigned char *key; //Key to be derived from Password
  int outSize;  //Size of the input file buffer
  int inSize;   //Size of the output file buffer
  FILE *inFile = fopen(input_file,"r");   //Declare inFile
  FILE *outFile = fopen(output_file, "w");//Declare outFile
  unsigned char *cmacFileMem = (unsigned char*)malloc(sizeof(unsigned char*)*16);//Buffer for cmac
  unsigned char *cmacFileMem2 = (unsigned char*)malloc(sizeof(unsigned char*)*16);//Buffer for cmac2
  /*Seek for input file length*/
  fseek(inFile, 0, SEEK_END);
  inSize = ftell(inFile);
  fseek(inFile, 0, SEEK_SET);
  printf("--------------------INFO-----------------------\n");
  printf("The size of the input file is %d bytes long\n", inSize);
	outSize = (inSize/BLOCK_SIZE + 1)*BLOCK_SIZE;
  /*allocate memmory for the input buffer */
  unsigned char *inFileMem = (unsigned char*)malloc(sizeof(unsigned char*)*inSize);
	unsigned char *outFileMem = (unsigned char*)malloc(sizeof(unsigned char*)*outSize);
  if (!fread(inFileMem,1,inSize,inFile)) {
    printf("-------Error passing file in memmory\n");
  }
  printf("                 HEX IN :\n");
  print_hex(inFileMem,inSize);
  /* Keygen from password */
    key = (unsigned char*)malloc(sizeof(unsigned char)*32);
    keygen(password, key, bit_mode);
  if (bit_mode==256) {
    printf("                HEX KEY:\n");
    print_hex (key, 32);
  } else if (bit_mode==128) {
    key = (unsigned char*)realloc(key,sizeof(unsigned char*)*16);
    printf("                HEX KEY:\n");
    print_hex (key, 16);
  } else {
    usage();
  }

	/* Operate on the data according to the mode */
  switch (op_mode) {
  case 0:
    /* encrypt */
    encrypt(inFileMem, inSize, key, outFileMem, bit_mode);
    fwrite(outFileMem, sizeof(char), outSize, outFile);
    printf("                 HEX OUT ENCRYPTED:\n");
    print_hex(outFileMem,outSize);
    printf("File %s renewed \n", output_file);
    break;
  case 1:
  	/* decrypt */
    outSize = decrypt(inFileMem, inSize, key, outFileMem, bit_mode);
    //outFileMem[outSize] = '\0';
    fwrite(outFileMem, sizeof(char), outSize, outFile);
    printf("                HEX OUT DECRYPTED:\n");
    print_hex(outFileMem,outSize);
    printf("--File %s renewed-- \n", output_file);
    break;
  case 2:
    /* encrypt */
    encrypt(inFileMem, inSize, key, outFileMem, bit_mode);
    printf("                HEX OUT ENCRYPTED:\n");
    print_hex(outFileMem,outSize);
  	/* generate cmac */
    gen_cmac(inFileMem, inSize, key, cmacFileMem, bit_mode);
    printf("                --CMAC-- \n");
    print_hex(cmacFileMem,16);
    /* sign */
    fwrite(outFileMem, sizeof(char), outSize, outFile);
    fwrite(cmacFileMem, sizeof(char), 16, outFile);
    break;
  case 3:
    printf("             --INPUT FILE-- \n");
    print_hex(inFileMem,inSize);
    inSize = inSize - 16;
    for (int i = 0; i < 16; i++) {
      cmacFileMem[i]=inFileMem[inSize+i];
    }
    printf("             --DERIVED CMAC-- \n");
    print_hex(cmacFileMem,16);
		/* decrypt */
		outSize = decrypt(inFileMem, inSize, key, outFileMem, bit_mode);
		printf("             HEX OUT DECRYPTED:\n");
		print_hex(outFileMem,outSize);
    //outFileMem[outSize] = '\0';
    gen_cmac(outFileMem, outSize, key, cmacFileMem2, bit_mode);
    printf("            --GENERATED CMAC-- \n");
    print_hex(cmacFileMem2,16);
  	/* verify */
    if (verify_cmac(cmacFileMem2,cmacFileMem)){
      fwrite(outFileMem, sizeof(char), outSize, outFile);
      printf("--File %s renewed-- \n", output_file);
    }else{
      printf("CMAC Verification error\n");
    }
    break;
  default:
		usage();
  }

	/* Clean up */
  fclose(inFile);
  fclose(outFile);
  free(cmacFileMem);
  free(cmacFileMem2);
  free(outFileMem);
  free(inFileMem);
	free(input_file);
	free(output_file);
	free(password);
  free(key);
	/* END */
	return 0;
}
