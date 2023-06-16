#include <stdlib.h>
#include <stdio.h>
#include "ripper.h"
#include "types.h"

boolean rip_texture64(u8bit *buff, const char *filename)
{
	FILE *bmp;
	int count;
	
	bmp = fopen(filename, "r");
	if (bmp == NULL) {
		printf("no file found\n");
		return no;
	}
	
	count = fseek(bmp, 1078, SEEK_SET);
	if (count != 0) {
		printf("header failed, %d\n", count);
		return no;
	}
	
	count = fread((void*)buff, sizeof(u8bit), 4096, bmp);
	if (count != 4096) {
		printf("image failed, %d of 4096\n", count);
		return no;
	}
	
	fclose(bmp);
	return yes;
}
