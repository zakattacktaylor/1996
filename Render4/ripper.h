#ifndef _RIPPER_H
#define _RIPPER_H

#include "types.h"

boolean rip_texture(u8bit *buff, const char *filename, int size);
boolean rip_palette(const char *filename);

#endif

