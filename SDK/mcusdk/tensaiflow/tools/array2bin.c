#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
typedef uint8_t q7_t;
#define __SYSRODATA__ 
#define ALIGN_TO(X)  
#include "input_data.h"

#define DBG
#define ARRAY_NAME(X) #X
#define ARRAY_2_FILE(X)   writeToFile(ARRAY_NAME(X),X, sizeof(X))
int writeToFile ( char * filename , const char * ptr , unsigned int size);

int count = 0x41;

int writeToFile ( char * name, const char * ptr , unsigned int size)
{
	char katanafilename[22] = {0};
	char filename[5] = {0};
	char* nameCopy = malloc(strlen(name) + 1);

	sprintf(nameCopy,"%c%c%c%c",count,count,count,count);
	count++;
	strcpy(katanafilename,"katana_");
	strcat(katanafilename,nameCopy);
	strcat(katanafilename,"_0000.blob");
	// printf("filename %s\n",katanafilename);
	int index = 0;	 
	FILE *  write_ptr=0;
	FILE *  read_ptr =0;
	char data = 0;
	write_ptr = fopen (katanafilename,"wb");
	if( !write_ptr )
		printf (" could not create file\n");
	else 
		fwrite(ptr,size,1,write_ptr); 
	fclose ( write_ptr);
#ifdef DBG
	read_ptr = fopen (katanafilename,"rb");
	for ( index =0 ;  index <size; index ++)
	{
		fread(&data,1,1,read_ptr);
	   // printf("%d\n", data);
		if (ptr[index] != data)
			printf (" ### Error in reading abck data\n");
	}
	fclose ( read_ptr);
#endif
	
}

void main(int argc, char* argv[])
{
// ARRAY_2_FILE_PLACEHOLDER
}