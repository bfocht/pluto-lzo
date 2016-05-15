#include <stdio.h>
#include <stdlib.h>
#include "minilzo.h"


/* exit codes */
#define EXIT_OK         0
#define EXIT_USAGE      1
#define EXIT_FILE       2
#define EXIT_MEM        3

int header_size = 4;

/*************************************************************************
//
**************************************************************************/
int main(int argc, char *argv[])
{
    lzo_uint src_len = 0;
    lzo_uint dst_len = 0;
    int i;
    unsigned char *source = NULL;  

 // initialize the LZO library
  if (lzo_init() != LZO_E_OK) {
    printf("internal error - lzo_init() failed !!!\n");
    printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
    return EXIT_MEM;
  }

// parse options
  if (argv == NULL || argc < 2 || 
    (argc > 2 && argv[1][0] != 'd')) 
  {
    printf("Usage: pluto-lzo {c|d} source_file dest_file\n");
    printf("       pluto-lzo compressed_file\n");
    return EXIT_USAGE;
  }

  char *dst_filename = NULL;
  char *src_filename = NULL;
  if (argc == 4) {
    
    dst_filename = argv[3];
    src_filename = argv[2];
  }
  else
  {
    src_filename = argv[1];
    int len = 0; 
    while(src_filename[len] != '\0') {
       len++; }
    if (len-- >5 &&
        src_filename[len--] == 'o' &&
        src_filename[len--] == 'z' &&
        src_filename[len--] == 'l' &&
        src_filename[len] == '.') {
 
        dst_filename = malloc(sizeof(char) * (len+1));
        for (i = 0; i < len; i++) {
          dst_filename[i] = src_filename[i]; 
        }
        dst_filename[len] = '\0';
      }
      else
      {
        fputs("Invalid input file, must end in .lzo\n", stderr);
        return EXIT_FILE;
      }
  }

  FILE *fp;
  fp = fopen(src_filename, "rb");
  if (fp == NULL) {
    fputs("Error opening file\n", stderr);
    return EXIT_FILE;
  }
    
  //read the header
  unsigned char buff[header_size];
  fread(buff, sizeof(unsigned char), header_size, fp); 
  for (i = 0; i < 4; i++) {
    dst_len += (int) buff[i] << (0x8 * i);
  }

  // read the remaining file into buffer
  // Go to the end of the file. 
  if (fseek(fp, 0L, SEEK_END) == 0) {
    /* Get the size of the file. */
    int bufsize = ftell(fp);
    if (bufsize == -1) { 
      fputs("Error reading file size\n", stderr);
      return EXIT_FILE;
    }
    /* Allocate our buffer to that size. */
    source =  malloc(sizeof(unsigned char) * (bufsize));
    /* Go to the start of the payload. */
    fseek(fp, 4L, SEEK_SET);
    /* Read the entire file into memory. */
    src_len = fread(source, sizeof(unsigned char), bufsize, fp);
    if (src_len == 0) {
        fputs("Error reading file into  memory\n", stderr);
        return EXIT_FILE;
    }
  }
  fclose(fp);
  

  //write the file
  unsigned char dst[dst_len];    
  lzo1x_decompress(source, src_len, dst, &dst_len, NULL);
  fp = fopen(dst_filename, "wb+");
  fwrite(dst, sizeof(unsigned char), dst_len, fp);
  fclose(fp);

  free(source);
  free(dst_filename);
  return EXIT_OK;
}
