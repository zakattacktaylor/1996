#include <stdlib.h>
#include <stdio.h>
#include "ripper.h"
#include "types.h"
#include "anim_sys.h"

typedef unsigned long DWORD;
typedef unsigned int WORD;
typedef unsigned char BYTE;

typedef struct tagBITMAPFILEHEADER {
        WORD    bfType; 
        DWORD   bfSize; 
        WORD    bfReserved1; 
        WORD    bfReserved2; 
        DWORD   bfOffBits; 
} BITMAPFILEHEADER; 

typedef struct tagBITMAPCOREHEADER {  
        DWORD   bcSize; 
        WORD    bcWidth; 
        WORD    bcHeight; 
        WORD    bcPlanes; 
        WORD    bcBitCount; 
} BITMAPCOREHEADER; 

typedef struct tagRGBTRIPLE { // rgbt  
    BYTE rgbtBlue; 
    BYTE rgbtGreen; 
    BYTE rgbtRed; 
} RGBTRIPLE; 

typedef struct _BITMAPCOREINFO {
        BITMAPCOREHEADER  bmciHeader; 
        RGBTRIPLE         bmciColors[1]; 
} BITMAPCOREINFO; 

boolean rip_texture(u8bit *buff, const char *filename, int size)
{
	FILE *bmp;
	size_t count;
	size_t sz = size * size;
	unsigned int offset;
	
	bmp = fopen(filename, "r");
	if (bmp == NULL) {
		printf("no file found\n");
		return no;
	}
	
	count = fseek(bmp, 1078, SEEK_SET);
	if (count != 0) {
		printf("header failed, %u\n", count);
		return no;
	}
	
	count = fread((void*)buff, sizeof(u8bit), sz, bmp);
	if (count != sz) {
		printf("image failed, %u of %u\n", count, sz);
		return no;
	}
	fclose(bmp);
	
	return yes;
}

boolean rip_palette(const char *filename)
{
	FILE *pal;
	size_t count;
	u8bit data[256*3];
	int i, k;
		
	pal = fopen(filename, "r");
	if (pal == NULL) {
		printf("no pal file found\n");
		return no;
	}
	
	count = fseek(pal, 0, SEEK_SET);
	if (count != 0) {
		printf("header failed, %u\n", count);
		return no;
	}
	
	count = fread((void*)data, sizeof(u8bit), 768, pal);
	if (count != 768) {
		printf("pal failed, %u of %u\n", count, 768);
		return no;
	}
	fclose(pal);

	for (i=0; i<256; i++) {
		k = 3 * i;
		//printf("pal[%d] = <%d, %d, %d>\n", i, data[k], data[k+1], data[k+2]);
		set_dac_register(i, data[k]/4, data[k+1]/4, data[k+2]/4);
	}
	return yes;
}

